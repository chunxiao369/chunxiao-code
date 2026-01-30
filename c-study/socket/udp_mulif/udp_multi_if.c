#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

int create_udp_socket(const char *ifname, int port) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        exit(1);
    }

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 关键：绑定到指定网卡
    if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname)) < 0) {
        perror("SO_BINDTODEVICE");
        exit(1);
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 必须 ANY

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    printf("UDP socket bound to %s:%d\n", ifname, port);
    return fd;
}

void handle_packet(int fd, const char *ifname) {
    char buf[2048];
    struct sockaddr_in src;
    socklen_t slen = sizeof(src);

    int n = recvfrom(fd, buf, sizeof(buf) - 1, 0,
                     (struct sockaddr *)&src, &slen);
    if (n < 0) {
        perror("recvfrom");
        return;
    }

    buf[n] = '\0';

    char src_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &src.sin_addr, src_ip, sizeof(src_ip));

    printf("[IN:%s] From %s:%d -> local port %d, len=%d, data=\"%s\"\n",
           ifname,
           src_ip,
           ntohs(src.sin_port),
           ntohs(((struct sockaddr_in){.sin_family=AF_INET,.sin_port=0}).sin_port), // 占位无意义
           n,
           buf);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        printf("Invalid port\n");
        return 1;
    }
    char *if0 = "enp0s31f6";
    char *if1 = "br0";

    int fd0 = create_udp_socket(if0, port);
    int fd1 = create_udp_socket(if1, port);

    fd_set rfds;
    int maxfd = (fd0 > fd1 ? fd0 : fd1) + 1;

    while (1) {
        FD_ZERO(&rfds);
        FD_SET(fd0, &rfds);
        FD_SET(fd1, &rfds);

        if (select(maxfd, &rfds, NULL, NULL, NULL) < 0) {
            if (errno == EINTR) continue;
            perror("select");
            break;
        }

        if (FD_ISSET(fd0, &rfds)) {
            handle_packet(fd0, if0);
        }
        if (FD_ISSET(fd1, &rfds)) {
            handle_packet(fd1, if1);
        }
    }

    close(fd0);
    close(fd1);
    return 0;
}

