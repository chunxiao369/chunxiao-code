/*
 * eBPF ip log demo
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bpf/libbpf.h>
#include <net/if.h>
#include <linux/if_link.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <interface>\n", argv[0]);
        return 1;
    }

    struct bpf_object *obj;
    struct bpf_program *prog;
    struct bpf_link *link;
    int ifindex = if_nametoindex(argv[1]);
    if (ifindex == 0) {
        fprintf(stderr, "Invalid interface: %s\n", argv[1]);
        return 1;
    }

    // 加载eBPF对象文件
    obj = bpf_object__open_file("ip_log.bpf.o", NULL);
    if (libbpf_get_error(obj)) {
        fprintf(stderr, "Failed to open BPF object\n");
        return 1;
    }

    // 加载程序
    if (bpf_object__load(obj)) {
        fprintf(stderr, "Failed to load BPF program\n");
        return 1;
    }

    // 找到程序
    prog = bpf_object__find_program_by_name(obj, "log_src_ip");
    if (!prog) {
        fprintf(stderr, "Failed to find BPF program\n");
        return 1;
    }

    // 附加到XDP钩子
    link = bpf_program__attach_xdp(prog, ifindex);
    if (libbpf_get_error(link)) {
        fprintf(stderr, "Failed to attach XDP program\n");
        return 1;
    }

    printf("Attached to %s! Press Ctrl-C to exit.\n", argv[1]);

    // 无限循环，等待信号
    while (1) {
        sleep(1);
    }

    // 清理（实际中用信号处理）
    bpf_link__destroy(link);
    bpf_object__close(obj);
    return 0;
}
