// xdp_tailcall.bpf.c
// clang -O2 -target bpf -c xdp_tailcall.bpf.c -o xdp_tailcall.bpf.o

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/in.h>

// Program array for tail calls
struct {
    __uint(type, BPF_MAP_TYPE_PROG_ARRAY);
    __uint(max_entries, 8);
    __uint(key_size, sizeof(int));
      __uint(value_size, sizeof(int));
} prog_array SEC(".maps");

// Simple per-cpu map for counting
struct {
    __uint(type, BPF_MAP_TYPE_PERCPU_ARRAY);
    __uint(max_entries, 4);
    __type(key, __u32);
    __type(value, __u64);
} pkt_count SEC(".maps");

// helper "function": increment counters (this is a normal function call inside BPF)
static __always_inline void count_packet(__u32 key)
{
    __u64 *val = bpf_map_lookup_elem(&pkt_count, &key);
    if (val) {
        __sync_fetch_and_add(val, 1);
    }
}

// Tail call target: handle TCP packets
SEC("xdp/tcp_prog")
     int xdp_tcp_prog(struct xdp_md *ctx)
{
    // Count at index 0
    count_packet(0);
    // For demo: drop TCP packets
    //return XDP_DROP;
    return XDP_PASS;
}

// Tail call target: handle UDP packets
SEC("xdp/udp_prog")
     int xdp_udp_prog(struct xdp_md *ctx)
{
    // Count at index 1
    count_packet(1);
    // For demo: pass UDP packets
    return XDP_PASS;
}

// Tail call target: handle other IPv4 packets
SEC("xdp/other_prog")
     int xdp_other_prog(struct xdp_md *ctx)
{
    // Count at index 2
    count_packet(2);
    // For demo: pass other packets
    return XDP_PASS;
}

// Main XDP program: parse basic headers and tail-call into protocol-specific progs
SEC("xdp/main")
     int xdp_main(struct xdp_md *ctx)
{
    void *data = (void *)(long)ctx->data;
    void *data_end = (void *)(long)ctx->data_end;

    // parse Ethernet header
    struct ethhdr *eth = data;
    if ((void *)(eth + 1) > data_end)
        return XDP_PASS;

    // only handle IPv4
    if (bpf_ntohs(eth->h_proto) != ETH_P_IP)
        return XDP_PASS;

    struct iphdr *ip = data + sizeof(struct ethhdr);
    if ((void *)(ip + 1) > data_end)
        return XDP_PASS;

    // basic sanity
    if (ip->ihl < 5)
        return XDP_PASS;

    // count all IPv4 packets at index 3
    count_packet(3);

    __u32 proto = ip->protocol;

    // Decide index for tail call: 0 -> TCP, 1 -> UDP, 2 -> other
    int index = 2;
    if (proto == IPPROTO_TCP)
        index = 0;
    else if (proto == IPPROTO_UDP)
        index = 1;

    // Try tail call: if the target index is not populated, control returns here
    bpf_tail_call(ctx, &prog_array, index);

    // If tail call failed (no program at index), fallback:
    return XDP_PASS;
}

char LICENSE[] SEC("license") = "GPL";
