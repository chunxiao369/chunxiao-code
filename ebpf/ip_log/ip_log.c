/*
 * eBPF ip log demo
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bpf/libbpf.h>
#include <net/if.h>
#include <linux/if_link.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

// 1. 定义 Ring Buffer 数据结构 (与 BPF 侧保持一致)
struct log_event {
    __u32 src_ip;
    int core_id;
};

static struct ring_buffer *rb = NULL;
static FILE *log_file = NULL;

// 信号处理函数，用于退出时清理
void sig_handler(int sig)
{
    printf("\nExiting and cleaning up...\n");
    if (rb) ring_buffer__free(rb);
    if (log_file) fclose(log_file);
    // 这里依赖 libbpf 自动清理 XDP 链接，或者由内核在程序退出时清理
    exit(0);
}

// 2. Ring Buffer 事件处理回调函数
static int handle_event(void *ctx, void *data, size_t len)
{
    const struct log_event *e = data;
    struct in_addr addr;
    
    if (len != sizeof(*e)) {
        fprintf(stderr, "Received incorrect event size: %zu\n", len);
        return 0;
    }

    // 转换 IP 地址为点分十进制
    addr.s_addr = e->src_ip;
    char ip_str[INET_ADDRSTRLEN];
    // 将网络字节序的 IP 转换为字符串
    if (inet_ntop(AF_INET, &addr, ip_str, INET_ADDRSTRLEN) == NULL) {
        strncpy(ip_str, "Unknown IP", INET_ADDRSTRLEN);
    }

    // 打印到终端 (可选)
    // printf("Received packet from IP: %s, core id: %d\n", ip_str, e->core_id);
    
    // 写入到指定的日志文件 /tmp/ip.log
    if (log_file) {
        // 使用 fprintf 写入文件
        fprintf(log_file, "Received packet from IP: %s, core id: %d\n", ip_str, e->core_id);
        fflush(log_file); // 确保数据立即写入磁盘
    }

    return 0;
}


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <interface>\n", argv[0]);
        return 1;
    }

    struct bpf_object *obj = NULL;
    struct bpf_link *link = NULL;
    int ifindex = if_nametoindex(argv[1]);
    if (ifindex == 0) {
        fprintf(stderr, "Invalid interface: %s\n", argv[1]);
        return 1;
    }

    // 设置信号处理
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    
    // 打开日志文件 /tmp/ip.log (追加模式)
    log_file = fopen("/tmp/ip.log", "a");
    if (!log_file) {
        perror("Failed to open /tmp/ip.log");
        return 1;
    }

    // 加载eBPF对象文件
    obj = bpf_object__open_file("ip_log.bpf.o", NULL);
    if (libbpf_get_error(obj)) {
        fprintf(stderr, "Failed to open BPF object\n");
        goto cleanup;
    }

    // 加载程序
    if (bpf_object__load(obj)) {
        fprintf(stderr, "Failed to load BPF program\n");
        goto cleanup;
    }

    // 附加到XDP钩子
    // 注意：libbpf 0.7+ 推荐使用 bpf_program__attach_xdp
    struct bpf_program *prog = bpf_object__find_program_by_name(obj, "log_src_ip");
    if (!prog) {
        fprintf(stderr, "Failed to find BPF program\n");
        goto cleanup;
    }
    
    link = bpf_program__attach_xdp(prog, ifindex);
    if (libbpf_get_error(link)) {
        fprintf(stderr, "Failed to attach XDP program: %s\n", strerror(libbpf_get_error(link)));
        link = NULL;
        goto cleanup;
    }
    
    // 找到 Ring Buffer Map 并设置事件处理
    int map_fd = bpf_object__find_map_fd_by_name(obj, "RINGBUF_MAP");
    if (map_fd < 0) {
        fprintf(stderr, "Failed to find Ring Buffer Map\n");
        goto cleanup;
    }

    // 创建 Ring Buffer 实例
    rb = ring_buffer__new(map_fd, handle_event, NULL, NULL);
    if (libbpf_get_error(rb)) {
        fprintf(stderr, "Failed to create ring buffer: %s\n", strerror(libbpf_get_error(rb)));
        rb = NULL;
        goto cleanup;
    }


    printf("Attached to %s! Press Ctrl-C to exit.\n", argv[1]);
    printf("Logging events to /tmp/ip.log\n");

    // 轮询 Ring Buffer
    while (1) {
        // 轮询 Ring Buffer，等待新的事件
        // timeout_ms = 100 意味着每 100ms 检查一次是否有新事件
        int err = ring_buffer__poll(rb, 100);
        
        // 如果出现错误或被信号中断，退出
        if (err < 0) {
            if (err == -EINTR) {
                continue; // 被信号中断，继续轮询
            }
            fprintf(stderr, "Error polling ring buffer: %s\n", strerror(-err));
            break;
        }
    }

cleanup:
    // 清理资源
    if (link) bpf_link__destroy(link);
    bpf_object__close(obj);
    if (rb) ring_buffer__free(rb);
    if (log_file) fclose(log_file);
    return 1;
}
