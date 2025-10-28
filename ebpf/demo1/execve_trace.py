from bcc import BPF

# eBPF程序（C代码嵌入Python）
program = r"""
int hello_exec(void *ctx) {
    bpf_trace_printk("Hello World!\\n");
    return 0;
}
"""

# 编译并加载eBPF程序
b = BPF(text=program)

# 附加到execve系统调用（kprobe）
syscall = b.get_syscall_fnname("execve")
b.attach_kprobe(event=syscall, fn_name="hello_exec")

# 输出提示
print("Tracing execve... Hit Ctrl-C to end.")

# 读取内核跟踪输出（需root权限）
b.trace_print()
