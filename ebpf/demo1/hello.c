/*
 * eBPF hello world demo
 */
#include <stdio.h>
#include <unistd.h>
#include <bpf/libbpf.h>

int main() {
    struct bpf_object *obj;
    struct bpf_program *prog;
    struct bpf_link *link;

    // 加载eBPF对象文件
    obj = bpf_object__open_file("hello.bpf.o", NULL);
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
    prog = bpf_object__find_program_by_name(obj, "hello_exec");
    if (!prog) {
        fprintf(stderr, "Failed to find BPF program\n");
        return 1;
    }

    // 附加kprobe
    link = bpf_program__attach(prog);
    if (libbpf_get_error(link)) {
        fprintf(stderr, "Failed to attach BPF program\n");
        return 1;
    }

    printf("Attached! Press Ctrl-C to exit.\n");

    // 无限循环，等待信号
    while (1) {
        sleep(1);
    }

    // 清理（实际中用信号处理）
    bpf_link__destroy(link);
    bpf_object__close(obj);
    return 0;
}
