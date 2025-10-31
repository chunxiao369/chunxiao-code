#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

//SEC("kprobe/__x64_sys_execve")
SEC("tracepoint/syscalls/sys_enter_execve")
int hello_exec(struct pt_regs *ctx) {
    bpf_printk("Hello, World!\n");
    return 0;
}

char _license[] SEC("license") = "GPL";

