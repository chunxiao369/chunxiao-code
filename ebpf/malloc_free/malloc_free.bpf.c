// malloc_free.bpf.c
// clang -O2 -target bpf -c malloc_free.bpf.c -o malloc_free.bpf.o

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_core_read.h>

// 用来统计 malloc/free 次数的结构体
struct counts {
    __u64 mallocs;
    __u64 frees;
};

// 使用 per-cpu hash，key: pid (u32)，val: struct counts
struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_HASH);
    __uint(max_entries, 10240);
    __type(key, __u32);
    __type(value, struct counts);
} pid_counts SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __uint(max_entries, 1);
    __type(key, __u32);
    __type(value, __u32);
} target_pid_map SEC(".maps");

// helper：获取或创建 key 的 entry（per-cpu）
static __always_inline struct counts *get_counts(__u32 pid)
{
    struct counts *c = bpf_map_lookup_elem(&pid_counts, &pid);
    if (!c) {
        struct counts zero = {};
        // 尝试 create：bpf_map_update_elem 可在内核端插入 new entry，但对于 per-cpu hash，
        // libbpf 验证会允许直接 lookup and if NULL then update with 0 to create.
        // 这里先用 bpf_map_update_elem 以创建初始值。
        bpf_map_update_elem(&pid_counts, &pid, &zero, BPF_NOEXIST);
        // 再次 lookup
        c = bpf_map_lookup_elem(&pid_counts, &pid);
    }
    return c;
}

// uprobe on malloc
SEC("uprobe/libc.so.6:malloc")
int uprobe_malloc(struct pt_regs *ctx)
{
    // get pid
    __u64 tid = bpf_get_current_pid_tgid();
    __u32 pid = ( __u32 )(tid >> 32);

    __u32 key = 0;
    __u32 *target = bpf_map_lookup_elem(&target_pid_map, &key);
    if (target && pid != *target)
        return 0; // 不是目标进程

    struct counts *c = get_counts(pid);
    if (c)
        __sync_fetch_and_add(&c->mallocs, 1);
    return 0;
}

// uprobe on free
SEC("uprobe/libc.so.6:free")
int uprobe_free(struct pt_regs *ctx)
{
    __u64 tid = bpf_get_current_pid_tgid();
    __u32 pid = ( __u32 )(tid >> 32);
    __u32 key = 0;
    __u32 *target = bpf_map_lookup_elem(&target_pid_map, &key);
    if (target && pid != *target)
        return 0;

    struct counts *c = get_counts(pid);
    if (c)
        __sync_fetch_and_add(&c->frees, 1);
    return 0;
}

char LICENSE[] SEC("license") = "GPL";

