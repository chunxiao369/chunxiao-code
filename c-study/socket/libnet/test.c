#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libnet.h>
#include <netinet/tcp.h>

// 配置参数（根据你的网络环境修改）
uint8_t src_mac[6] = {0x00, 0x0c, 0x29, 0xaa, 0xbb, 0xcc};  // 本机MAC
uint8_t dst_mac[6] = {0x00, 0x50, 0x56, 0xdd, 0xee, 0xff}; // 对端MAC
#define SRC_IP "192.168.1.100"   // 源IP
#define DST_IP "192.168.1.200"   // 目标IP
#define SRC_PORT 12345           // 源端口
#define DST_PORT 8080            // 目标端口
#define PAYLOAD "Hello from libnet!"  // 自定义Payload

int main() {
    char errbuf[LIBNET_ERRBUF_SIZE];
    libnet_t *l = NULL;
    libnet_ptag_t tcp_tag, ip_tag;
    u_long src_ip, dst_ip;
    libnet_ptag_t eth_tag;

    // 1. 初始化libnet上下文（链路层）
    l = libnet_init(LIBNET_LINK, NULL, errbuf);
    if (l == NULL) {
        fprintf(stderr, "libnet_init failed: %s\n", errbuf);
        return -1;
    }


    // 2. 转换IP地址格式
    src_ip = libnet_name2addr4(l, SRC_IP, LIBNET_RESOLVE);
    dst_ip = libnet_name2addr4(l, DST_IP, LIBNET_RESOLVE);
    if (src_ip == -1 || dst_ip == -1) {
        fprintf(stderr, "Invalid IP address\n");
        libnet_destroy(l);
        return -1;
    }

    // ========== 第一步：生成SYN包（第一次握手） ==========
    // 构建TCP头部（SYN标志位）
    tcp_tag = libnet_build_tcp(
        SRC_PORT,               // 源端口
        DST_PORT,               // 目标端口
        123456,                 // 序列号（随机即可）
        0,                      // 确认号（SYN包为0）
        TH_SYN,                 // TCP标志位：仅SYN
        1024,                   // 窗口大小
        0,                      // 校验和（libnet自动计算）
        0,                      // 紧急指针
        LIBNET_TCP_H,           // TCP头部长度
        NULL,                   // SYN包无Payload
        0,                      // Payload长度
        l,                      // libnet上下文
        0                       // 复用ptag（0为新建）
    );
    if (tcp_tag == -1) {
        fprintf(stderr, "Build SYN TCP header failed: %s\n", libnet_geterror(l));
        libnet_destroy(l);
        return -1;
    }

    // 构建IP头部
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H,  // IP总长度（IP头+TCP头）
        0,                              // TOS
        54321,                          // ID
        0,                              // 分片偏移
        64,                             // TTL
        IPPROTO_TCP,                    // 协议：TCP
        0,                              // 校验和（libnet自动计算）
        src_ip,                         // 源IP
        dst_ip,                         // 目标IP
        NULL,                           // 无额外数据
        0,                              // 数据长度
        l,                              // libnet上下文
        0                               // 复用ptag
    );
    if (ip_tag == -1) {
        fprintf(stderr, "Build SYN IP header failed: %s\n", libnet_geterror(l));
        libnet_destroy(l);
        return -1;
    }

    eth_tag = libnet_build_ethernet(
        dst_mac,              // 目的 MAC
        src_mac,              // 源 MAC
        ETHERTYPE_IP,         // 上层协议：IP
        NULL,                 // payload
        0,                    // payload size
        l,                    // libnet handle
        0                     // ptag
    );

    if (eth_tag == -1) {
        fprintf(stderr, "libnet_build_ethernet failed: %s\n",
                libnet_geterror(l));
        return -1;
    }


    // 发送SYN包
    int bytes = libnet_write(l);
    if (bytes == -1) {
        fprintf(stderr, "Send SYN packet failed: %s\n", libnet_geterror(l));
    } else {
        printf("Sent SYN packet (first handshake): %d bytes\n", bytes);
    }

    // ========== 第三步：生成带Payload的ACK+PSH包（握手后数据报） ==========
    // 重置libnet上下文，准备构建新包
    libnet_clear_packet(l);

    // 构建TCP头部（ACK+PSH标志位，携带Payload）
    tcp_tag = libnet_build_tcp(
        SRC_PORT,               // 源端口
        DST_PORT,               // 目标端口
        123457,                 // 序列号（比SYN包+1）
        765432,                 // 确认号（需匹配服务端的SYN+ACK序列号）
        TH_ACK | TH_PUSH,       // TCP标志位：ACK（确认连接）+ PUSH（推送数据）
        1024,                   // 窗口大小
        0,                      // 校验和
        0,                      // 紧急指针
        LIBNET_TCP_H,           // TCP头部长度
        (u_char *)PAYLOAD,      // 自定义Payload
        strlen(PAYLOAD),        // Payload长度
        l,                      // libnet上下文
        0                       // 新建ptag
    );
    if (tcp_tag == -1) {
        fprintf(stderr, "Build data TCP header failed: %s\n", libnet_geterror(l));
        libnet_destroy(l);
        return -1;
    }

    // 构建IP头部（包含Payload）
    ip_tag = libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H + strlen(PAYLOAD),  // IP总长度（含Payload）
        0,                                                // TOS
        54322,                                            // ID
        0,                                                // 分片偏移
        64,                                               // TTL
        IPPROTO_TCP,                                      // 协议：TCP
        0,                                                // 校验和
        src_ip,                                           // 源IP
        dst_ip,                                           // 目标IP
        NULL,                                             // 无额外数据
        0,                                                // 数据长度
        l,                                                // libnet上下文
        0                                                 // 复用ptag
    );
    if (ip_tag == -1) {
        fprintf(stderr, "Build data IP header failed: %s\n", libnet_geterror(l));
        libnet_destroy(l);
        return -1;
    }

    eth_tag = libnet_build_ethernet(
        dst_mac,              // 目的 MAC
        src_mac,              // 源 MAC
        ETHERTYPE_IP,         // 上层协议：IP
        NULL,                 // payload
        0,                    // payload size
        l,                    // libnet handle
        0                     // ptag
    );

    if (eth_tag == -1) {
        fprintf(stderr, "libnet_build_ethernet failed: %s\n",
                libnet_geterror(l));
        return -1;
    }


    // 发送带Payload的数据报
    bytes = libnet_write(l);
    if (bytes == -1) {
        fprintf(stderr, "Send data packet failed: %s\n", libnet_geterror(l));
    } else {
        printf("Sent data packet with payload: %d bytes, payload: %s\n", bytes, PAYLOAD);
    }

    // 3. 释放资源
    libnet_destroy(l);
    return 0;
}
