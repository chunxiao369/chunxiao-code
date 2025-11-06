#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>

SEC("xdp")
int log_src_ip(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;
    struct ethhdr *eth = data;
    struct iphdr *iph;

    int id = bpf_get_smp_processor_id();
    // 检查以太网头部
    if (data + sizeof(*eth) > data_end)
        return XDP_PASS;

    // 只处理IPv4
    if (bpf_ntohs(eth->h_proto) != ETH_P_IP)
        return XDP_PASS;

    // 检查IP头部
    iph = data + sizeof(*eth);
    if ((void *)iph + sizeof(*iph) > data_end)
        return XDP_PASS;

    // 提取源IP（对端IP）
    //__u32 src_ip = bpf_ntohl(iph->saddr);
    __u32 src_ip = iph->saddr;
    if (src_ip == 0x356e3c0a) {
        //10.60.110.53该ip的数据包即被在进入协议栈之前丢弃
        return XDP_DROP;
    }

    // 打印日志（格式：源IP的点分十进制）
    bpf_printk("Received packet from IP: %pI4, core id: %d\n", &src_ip, id);

    return XDP_PASS;  // 允许包继续处理
}

char _license[] SEC("license") = "GPL";

