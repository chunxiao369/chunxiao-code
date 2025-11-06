#include <linux/bpf.h>
//#include <bpf/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>
#include <bpf/bpf_tracing.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>

// 1. 定义 Ring Buffer 数据结构 (用于传输源IP和核心ID)
struct log_event {
    __u32 src_ip;
    int core_id;
};

// 2. 定义 Ring Buffer Map (.maps 是一个特殊的 BPF ELF 段)
struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024); // 256KB 环形缓冲区
} RINGBUF_MAP SEC(".maps");


SEC("xdp")
int log_src_ip(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;
    struct ethhdr *eth = data;
    struct iphdr *iph;

    // 获取当前处理的核心 ID
    int id = bpf_get_smp_processor_id();
    
    // 边界检查：以太网头部
    if (data + sizeof(*eth) > data_end)
        return XDP_PASS;

    // 只处理IPv4
    if (bpf_ntohs(eth->h_proto) != ETH_P_IP)
        return XDP_PASS;

    // 边界检查：IP 头部
    iph = data + sizeof(*eth);
    if ((void *)iph + sizeof(*iph) > data_end)
        return XDP_PASS;

    // 提取源IP (网络字节序)
    __u32 src_ip = iph->saddr;
    
    // --- 替换 bpf_printk 为 Ring Buffer 输出 ---
    struct log_event *event;

    // 1. 在 Ring Buffer 中保留空间
    // bpf_ringbuf_reserve 会尝试原子性地在环形缓冲区中预留一个事件空间
    event = bpf_ringbuf_reserve(&RINGBUF_MAP, sizeof(*event), 0);
    if (event) {
        // 2. 填充数据
        event->src_ip = src_ip;
        event->core_id = id;
        
        // 3. 提交数据
        bpf_ringbuf_submit(event, 0);
    }
    // ---------------------------------------------
    
    // 丢弃指定IP的数据包 (10.60.110.53)
    if (src_ip == bpf_htonl(0x0a3c6e35)) {
        return XDP_DROP;
    }

    return XDP_PASS;  // 允许包继续处理
}

char _license[] SEC("license") = "GPL";

