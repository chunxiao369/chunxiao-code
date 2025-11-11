// malloc_free_user.c
// gcc -O2 malloc_free_user.c -o malloc_free_user -lbpf -lelf -lz -pthread

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>

#include <bpf/libbpf.h>
#include <bpf/bpf.h>

static volatile bool exiting = false;
static void sigint(int sig) { exiting = true; }

// helper: raise memlock rlimit
static int bump_memlock_rlimit(void)
{
    struct rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
    if (setrlimit(RLIMIT_MEMLOCK, &r)) {
        perror("setrlimit(RLIMIT_MEMLOCK)");
        return -1;
    }
    return 0;
}

// find libc path by reading /proc/<pid>/maps and locating libc.so.6 mapping
static int find_libc_path_for_pid(pid_t pid, char *buf, size_t buflen)
{
    char maps_path[64];
    snprintf(maps_path, sizeof(maps_path), "/proc/%d/maps", pid);
    FILE *f = fopen(maps_path, "r");
    if (!f) return -1;

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        // typical line ends with path like /lib/x86_64-linux-gnu/libc-2.31.so
        // we match libc-*.so or libc.so.6 filename
        if (strstr(line, "libc-") || strstr(line, "libc.so.6")) {
            // parse last token
            char *p = strrchr(line, '/');
            if (!p) continue;
            // trim newline
            char *nl = strchr(p, '\n');
            if (nl) *nl = 0;
            // copy entire path starting from '/' to end
            // find first '/' before filename in line
            char *slash = strchr(line, '/');
            if (slash) {
                strncpy(buf, slash, buflen - 1);
                buf[buflen - 1] = 0;
                fclose(f);
                return 0;
            }
        }
    }
    fclose(f);
    return -1;
}

// struct counts in userspace: reproduce layout
struct counts {
    unsigned long long mallocs;
    unsigned long long frees;
};
int main(int argc, char **argv)
{
    struct bpf_object *obj = NULL;
    int map_fd = -1;
    pid_t target_pid;
    char libc_path[512] = {0};
    int err;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        return 1;
    }
    target_pid = atoi(argv[1]);
    if (target_pid <= 0) {
        fprintf(stderr, "invalid pid\n");
        return 1;
    }

    if (bump_memlock_rlimit()) return 1;

    // locate libc for the target pid
    if (find_libc_path_for_pid(target_pid, libc_path, sizeof(libc_path)) != 0) {
        fprintf(stderr, "Failed to find libc path for pid %d. You may need to supply path manually.\n", target_pid);
        return 1;
    }
    printf("Found libc for pid %d: %s\n", target_pid, libc_path);

    // open BPF object file compiled from malloc_free.bpf.c
    obj = bpf_object__open_file("malloc_free.bpf.o", NULL);
    if (!obj) {
        fprintf(stderr, "Failed to open BPF object\n");
        return 1;
    }

    // load
    if (bpf_object__load(obj)) {
        fprintf(stderr, "Failed to load BPF object\n");
        goto cleanup;
    }

    // find map fd
    struct bpf_map *map = bpf_object__find_map_by_name(obj, "pid_counts");
    if (!map) {
        fprintf(stderr, "Failed to find map pid_counts\n");
        goto cleanup;
    }
    map_fd = bpf_map__fd(map);

    // attach uprobes: find programs by section name in object
    struct bpf_program *prog_malloc = bpf_object__find_program_by_name(obj, "uprobe_malloc");
    struct bpf_program *prog_free   = bpf_object__find_program_by_name(obj, "uprobe_free");
    if (!prog_malloc || !prog_free) {
        fprintf(stderr, "Failed to find uprobe programs in object\n");
        goto cleanup;
    }

    struct bpf_map *pid_map = bpf_object__find_map_by_name(obj, "target_pid_map");
    int pid_map_fd = bpf_map__fd(pid_map);
    __u32 key0 = 0;
    bpf_map_update_elem(pid_map_fd, &key0, &target_pid, BPF_ANY);

    // Attach uprobe to malloc symbol in libc for target pid (attach to that pid's binary)
    // libbpf provides bpf_program__attach_uprobe API
    struct bpf_link *link_malloc = NULL;
    struct bpf_link *link_free = NULL;

    // attach uprobe to malloc
    // prototype: bpf_program__attach_uprobe(prog, bool retprobe, const char *binary_path, const char *symbol, pid_t pid);
    link_malloc = bpf_program__attach_uprobe(prog_malloc, false, libc_path, "malloc", -1);
    if (!link_malloc) {
        fprintf(stderr, "Failed to attach uprobe malloc: %s\n", strerror(errno));
        goto cleanup;
    }

    // attach uprobe to free
    link_free = bpf_program__attach_uprobe(prog_free, false, libc_path, "free", -1);
    if (!link_free) {
        fprintf(stderr, "Failed to attach uprobe free: %s\n", strerror(errno));
        bpf_link__destroy(link_malloc);
        goto cleanup;
    }


    printf("Attached to malloc/free on %s for pid %d\n", libc_path, target_pid);

    signal(SIGINT, sigint);
    signal(SIGTERM, sigint);

    // poll loop: every 2s read per-cpu hash for the pid and aggregate counts
    while (!exiting) {
        sleep(2);

        __u32 key = ( __u32 ) target_pid;

        // determine number of possible cpus
        int ncpu = libbpf_num_possible_cpus();
        if (ncpu <= 0) ncpu = 1;
        size_t valsize = sizeof(struct counts) * ncpu;
        void *buf = malloc(valsize);
        if (!buf) {
            perror("malloc");
            break;
        }
        memset(buf, 0, valsize);

        // bpf_map_lookup_elem will copy per-cpu array (ncpu elements) into buf
        if (bpf_map_lookup_elem(map_fd, &key, buf) != 0) {
            // not found => zero
            printf("pid %-6u malloc: %-10llu free: %-10llu (no entry yet)\n",
                   target_pid, 0ULL, 0ULL);
            free(buf);
            continue;
        }

        // sum across cpus
        unsigned long long total_malloc = 0;
        unsigned long long total_free = 0;

        for (int i = 0; i < ncpu; i++) {
            struct counts *c = (struct counts *)((char *)buf + i * sizeof(struct counts));
            total_malloc += c->mallocs;
            total_free  += c->frees;
        }

        printf("pid %-6u malloc: %-10llu free: %-10llu\n",
               target_pid, total_malloc, total_free);

        free(buf);
    }

    printf("Detaching and exiting...\n");

    if (link_free) bpf_link__destroy(link_free);
    if (link_malloc) bpf_link__destroy(link_malloc);

cleanup:
    if (obj) bpf_object__close(obj);
    return 0;
}

