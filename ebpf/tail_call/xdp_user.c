// xdp_user.c
// gcc -O2 xdp_user.c -o xdp_user -lbpf -lelf -lz -pthread

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/resource.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include <net/if.h>
#include <linux/if_link.h>

static volatile bool exiting = false;
static void sigint_handler(int sig)
{
    exiting = true;
}

static int bump_memlock_rlimit(void)
{
    struct rlimit rlim_new = {
        .rlim_cur = RLIM_INFINITY,
        .rlim_max = RLIM_INFINITY,
    };
    if (setrlimit(RLIMIT_MEMLOCK, &rlim_new)) {
        perror("setrlimit(RLIMIT_MEMLOCK)");
        return -1;
    }
    return 0;
}

const char *pkt_str[] = {
    "tcp",
    "udp",
    "other",
    "total"
};

int main(int argc, char **argv)
{
    struct bpf_object *obj = NULL;
    int prog_main_fd = -1;
    int prog_tcp_fd = -1, prog_udp_fd = -1, prog_other_fd = -1;
    int prog_array_fd = -1;
    int pkt_count_fd = -1;
    int ifindex = 0;
    const char *ifname;
    char filename[256];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <ifname>\n", argv[0]);
        return 1;
    }
    ifname = argv[1];
    ifindex = if_nametoindex(ifname);
    if (!ifindex) {
        perror("if_nametoindex");
        return 1;
    }

    if (bump_memlock_rlimit())
        return 1;

    // Open BPF object
    snprintf(filename, sizeof(filename), "./xdp_tailcall.bpf.o");
    obj = bpf_object__open_file(filename, NULL);
    if (!obj) {
        fprintf(stderr, "Failed to open BPF object file: %s\n", filename);
        return 1;
    }

    if (bpf_object__load(obj)) {
        fprintf(stderr, "Failed to load BPF object\n");
        goto cleanup;
    }
    // Find program fds by section name
    struct bpf_program *p;

    p = bpf_object__find_program_by_name(obj, "xdp_main");
    if (!p) {
        fprintf(stderr, "cannot find xdp/main\n");
        goto cleanup;
    }
    prog_main_fd = bpf_program__fd(p);

    p = bpf_object__find_program_by_name(obj, "xdp_tcp_prog");
    if (!p) {
        fprintf(stderr, "cannot find xdp/tcp_prog\n");
        goto cleanup;
    }
    prog_tcp_fd = bpf_program__fd(p);

    p = bpf_object__find_program_by_name(obj, "xdp_udp_prog");
    if (!p) {
        fprintf(stderr, "cannot find xdp/udp_prog\n");
        goto cleanup;
    }
    prog_udp_fd = bpf_program__fd(p);

    p = bpf_object__find_program_by_name(obj, "xdp_other_prog");
    if (!p) {
        fprintf(stderr, "cannot find xdp/other_prog\n");
        goto cleanup;
    }
    prog_other_fd = bpf_program__fd(p);

    // Get map fds
    struct bpf_map *m;
    m = bpf_object__find_map_by_name(obj, "prog_array");
    if (!m) {
        fprintf(stderr, "cannot find map prog_array\n");
        goto cleanup;
    }
    prog_array_fd = bpf_map__fd(m);

    m = bpf_object__find_map_by_name(obj, "pkt_count");
    if (!m) {
        fprintf(stderr, "cannot find map pkt_count\n");
        goto cleanup;
    }
    pkt_count_fd = bpf_map__fd(m);

    // Update prog_array: index 0 -> tcp, 1 -> udp, 2 -> other
    if (bpf_map_update_elem(prog_array_fd, &(int) { 0 }, &prog_tcp_fd, BPF_ANY) < 0) {
        perror("bpf_map_update_elem tcp");
        goto cleanup;
    }
    if (bpf_map_update_elem(prog_array_fd, &(int) { 1 }, &prog_udp_fd, BPF_ANY) < 0) {
        perror("bpf_map_update_elem udp");
        goto cleanup;
    }
    if (bpf_map_update_elem(prog_array_fd, &(int) { 2 }, &prog_other_fd, BPF_ANY) < 0) {
        perror("bpf_map_update_elem other");
        goto cleanup;
    }
    // Attach the main XDP program to the interface
    int flags = XDP_FLAGS_UPDATE_IF_NOEXIST | XDP_FLAGS_DRV_MODE;   // try driver mode
    if (bpf_set_link_xdp_fd(ifindex, prog_main_fd, flags) < 0) {
        // Try generic mode fallback
        fprintf(stderr, "Driver XDP attach failed, try generic\n");
        flags = XDP_FLAGS_UPDATE_IF_NOEXIST | XDP_FLAGS_SKB_MODE;
        if (bpf_set_link_xdp_fd(ifindex, prog_main_fd, flags) < 0) {
            perror("bpf_set_link_xdp_fd");
            goto cleanup;
        }
    }

    printf("XDP program attached on %s (ifindex=%d). Press Ctrl-C to stop.\n", ifname, ifindex);
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);

    int ncpu = libbpf_num_possible_cpus();  // 获取系统CPU数量
    // Periodically read pkt_count map and print
    while (!exiting) {
        sleep(2);
        __u32 key;
        for (key = 0; key < 4; ++key) {
            __u64 values[ncpu];
            int ret = 0;
            __u64 total = 0;
            ret = bpf_map_lookup_elem(pkt_count_fd, &key, values);
            if (0 == ret) {
                for (int i = 0; i < ncpu; i++) {
                    total += values[i];
                }
                printf("[%s] counter = %llu\n", pkt_str[key], (unsigned long long)total);
            } else {
                perror("bpf_map_lookup_elem");
            }
        }
        printf("----\n");
    }

    printf("Detaching XDP program from ifindex %d\n", ifindex);
    bpf_set_link_xdp_fd(ifindex, -1, flags);    // detach

cleanup:
    if (obj)
        bpf_object__close(obj);
    return 0;
}
