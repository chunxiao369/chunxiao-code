// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <bpf/libbpf.h>
#include <libelf.h>
#include <gelf.h>
#include "malloc_free.skel.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

static volatile int exiting = 0;

struct count_t {
    __u64 mallocs;
    __u64 frees;
};
static void sig_handler(int signo)
{
    exiting = 1;
}

// get_symbol_offset 函数保持不变 (和上一版一样)
size_t get_symbol_offset(const char *binary, const char *symbol_name) {
    Elf *elf;
    GElf_Ehdr ehdr;
    Elf_Scn *scn = NULL;
    GElf_Shdr shdr;
    Elf_Data *data;
    size_t offset = 0;
    int fd = open(binary, O_RDONLY);
    if (fd < 0) {
	printf("open error!\n");
        return 0;
    }

    elf_version(EV_CURRENT);
    elf = elf_begin(fd, ELF_C_READ, NULL);
    if (!elf) {
	printf("begin error!\n");
        goto out;
    }

    if (gelf_getehdr(elf, &ehdr) == NULL) {
	printf("getehdr error!\n");
        goto out;
    }

    while ((scn = elf_nextscn(elf, scn)) != NULL) {
        gelf_getshdr(scn, &shdr);
        if ( shdr.sh_type== SHT_DYNSYM || shdr.sh_type== SHT_SYMTAB) {
            data = elf_getdata(scn, NULL);
            int count = shdr.sh_size / shdr.sh_entsize;
            for (int i = 0; i < count; i++) {
                GElf_Sym sym;
                gelf_getsym(data, i, &sym);
                const char *name = elf_strptr(elf, shdr.sh_link, sym.st_name);
                if (name && strcmp(name, symbol_name) == 0) {
                    offset = sym.st_value;
                    goto out;
                }
            }
        }
    }

out:
    elf_end(elf);
    close(fd);
    return offset;
}


int main(int argc, char **argv)
{
    struct malloc_free_bpf *skel;
    struct count_t count = {};
    __u32 key0 = 0, pid;
    int err;
    pid_t child;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <program>\n", argv[0]);
        return 1;
    }

    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    // 启动目标程序
    child = fork();
    if (child == 0) {
        // 子进程: 停止自己，等待父进程设置 BPF
        raise(SIGSTOP);
        execvp(argv[1], &argv[1]);
        perror("execvp failed");
        exit(1);
    }

    printf("Target process PID: %d\n", child);
    pid = child;

    // 父进程: 等待子进程停止
    int status;
    waitpid(child, &status, WUNTRACED);
    if (!WIFSTOPPED(status)) {
        fprintf(stderr, "Child process did not stop?\n");
        return 1;
    }
    printf("Child process stopped. Setting up BPF...\n");

    // 1. 打开 eBPF 程序
    skel = malloc_free_bpf__open();
    if (!skel) {
        fprintf(stderr, "Failed to open BPF skeleton\n");
        return 1;
    }

    // *** 关键修改：先 LOAD 再 UPDATE ***

    // 2. 加载和验证 BPF 程序 (这会创建 maps)
    err = malloc_free_bpf__load(skel);
    if (err) {
        fprintf(stderr, "Failed to load BPF skeleton: %d\n", err);
        goto cleanup_and_cont;
    }

    // 3. 现在 maps 有效了，设置目标 pid
    err = bpf_map_update_elem(bpf_map__fd(skel->maps.target_pid), &key0, &pid, BPF_ANY);
    if (err) {
        fprintf(stderr, "Failed to update target_pid map: %s\n", strerror(errno));
        goto cleanup_and_cont;
    }

    // 4. 找到 libc 路径
    const char *libc_path = "/lib/x86_64-linux-gnu/libc.so.6";
    if (access(libc_path, F_OK) != 0)
        libc_path = "/usr/lib/x86_64-linux-gnu/libc.so.6";
    if (access(libc_path, F_OK) != 0) {
        fprintf(stderr, "Could not find libc.so.6\n");
        goto cleanup_and_cont;
    }

    // 5. 查找符号偏移
    size_t malloc_off = get_symbol_offset(libc_path, "malloc");
    size_t free_off   = get_symbol_offset(libc_path, "free");

    if (!malloc_off)
        malloc_off = get_symbol_offset(libc_path, "__libc_malloc");
    if (!free_off)
        free_off = get_symbol_offset(libc_path, "__libc_free");


    if (!malloc_off || !free_off) {
        fprintf(stderr, "Failed to find symbol offsets\n");
        goto cleanup_and_cont;
    }
    printf("Found offsets: malloc=0x%lx, free=0x%lx.\n", malloc_off, free_off);

    // 6. 挂载 uprobes
    skel->links.uprobe_malloc = bpf_program__attach_uprobe(
        skel->progs.uprobe_malloc, false, -1, libc_path, malloc_off);
    if (!skel->links.uprobe_malloc) {
        fprintf(stderr, "Failed to attach malloc uprobe: %s\n", strerror(errno));
        goto cleanup_and_cont;
    }

    skel->links.uprobe_free = bpf_program__attach_uprobe(
        skel->progs.uprobe_free, false, -1, libc_path, free_off);
    if (!skel->links.uprobe_free) {
        fprintf(stderr, "Failed to attach free uprobe: %s\n", strerror(errno));
        goto cleanup_and_cont;
    }

    printf("Tracing malloc/free calls for PID %d ...\n", pid);
    printf("Press Ctrl+C to stop.\n");

    // 7. 告诉子进程继续
    if (kill(child, SIGCONT) < 0) {
        perror("kill(SIGCONT)");
        goto cleanup;
    }

    // 8. 循环读取
    while (!exiting) {
        sleep(2);
        if (bpf_map_lookup_elem(bpf_map__fd(skel->maps.counts), &pid, &count) == 0)
            printf("PID %d: malloc=%llu free=%llu\n",
                   pid, count.mallocs, count.frees);
        else
            printf("PID %d: no data yet\n", pid);

        if (waitpid(child, &status, WNOHANG) == child) {
            printf("Target process exited.\n");
            // 最后再读取一次
            if (bpf_map_lookup_elem(bpf_map__fd(skel->maps.counts), &pid, &count) == 0)
                printf("PID %d: malloc=%llu free=%llu\n",
                       pid, count.mallocs, count.frees);
            break;
        }
    }

cleanup:
    malloc_free_bpf__destroy(skel);
    return 0;

cleanup_and_cont:
    // 确保子进程不会永远停止
    kill(child, SIGCONT);
    malloc_free_bpf__destroy(skel);
    return 1;
}
