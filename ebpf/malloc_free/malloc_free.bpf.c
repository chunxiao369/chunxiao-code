// SPDX-License-Identifier: GPL-2.0
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
//#include "vmlinux.h"

struct count_t {
    __u64 mallocs;
    __u64 frees;
};

// 保存目标 pid
struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __uint(max_entries, 1);
    __type(key, __u32);
    __type(value, __u32);
} target_pid SEC(".maps");

// 每个 pid 的统计
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, __u32);
    __type(value, struct count_t);
} counts SEC(".maps");

static __always_inline struct count_t *get_count(__u32 pid) {
    struct count_t *c = bpf_map_lookup_elem(&counts, &pid);
    if (!c) {
        struct count_t zero = {};
        bpf_map_update_elem(&counts, &pid, &zero, BPF_ANY);
        c = bpf_map_lookup_elem(&counts, &pid);
    }
    return c;
}

SEC("uprobe/libc.so.6:malloc")
int uprobe_malloc(struct pt_regs *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 pid = id >> 32;
    //bpf_printk("malloc called\n");

    __u32 key = 0;
    __u32 *target = bpf_map_lookup_elem(&target_pid, &key);
    if (!target || pid != *target)
        return 0;

    struct count_t *c = get_count(pid);
    if (c)
        __sync_fetch_and_add(&c->mallocs, 1);
    return 0;
}

SEC("uprobe/libc.so.6:free")
int uprobe_free(struct pt_regs *ctx)
{
    __u64 id = bpf_get_current_pid_tgid();
    __u32 pid = id >> 32;
    //bpf_printk("free called\n");

    __u32 key = 0;
    __u32 *target = bpf_map_lookup_elem(&target_pid, &key);
    //bpf_printk("Hello, free!\n");
    if (!target || pid != *target)
        return 0;

    struct count_t *c = get_count(pid);
    if (c)
        __sync_fetch_and_add(&c->frees, 1);
    return 0;
}

char LICENSE[] SEC("license") = "GPL";

