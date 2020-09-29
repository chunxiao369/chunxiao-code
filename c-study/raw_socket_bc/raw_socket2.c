/**
 * @file raw_socket1.c
 * @Synopsis
 * @author xu.chunxiao@embedway.com
 * @version 1.0
 * @date 2011-12-06
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>

/* 接收缓冲区大小 */
#define RCV_BUF_SIZE     1024 * 5

/* 接收缓冲区 */
static int g_iRecvBufSize = RCV_BUF_SIZE;

/* 输出MAC地址 */
/* 物理网卡混杂模式属性操作 */
static int ethdump_setPromisc(const char *pcIfName, int fd, int iFlags)
{
    int iRet = -1;
    int flags;
    struct ifreq stIfr;

    /* 获取接口属性标志位 */
    strcpy(stIfr.ifr_name, pcIfName);
    iRet = ioctl(fd, SIOCGIFFLAGS, &stIfr);
    if (0 > iRet) {
        perror("[Error]Get Interface Flags");
        return -1;
    }
    flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    if (0 == iFlags) {
        /* 取消混杂模式 */
        stIfr.ifr_flags &= ~IFF_PROMISC;
    } else {
        /* 设置为混杂模式 */
        stIfr.ifr_flags |= IFF_PROMISC;
    }

    iRet = ioctl(fd, SIOCSIFFLAGS, &stIfr);
    if (0 > iRet) {
        perror("[Error]Set Interface Flags");
        return -1;
    }

    return 0;
}

static int ethdump_initSocket(char *g_szIfName)
{
    int iRet = -1;
    int fd = -1;
    struct ifreq stIf;
    struct sockaddr_ll stLocal = { 0 };

    /* 创建SOCKET */
    fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (0 > fd) {
        perror("[Error]Initinate L2 raw socket");
        return -1;
    }

    /* 网卡混杂模式设置 */
    ethdump_setPromisc(g_szIfName, fd, 1);

    /* 设置SOCKET选项 */
    iRet = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &g_iRecvBufSize, sizeof(int));
    if (0 > iRet) {
        perror("[Error]Set socket option");
        close(fd);
        return -1;
    }

    /* 获取物理网卡接口索引 */
    strcpy(stIf.ifr_name, g_szIfName);
    iRet = ioctl(fd, SIOCGIFINDEX, &stIf);
    if (0 > iRet) {
        perror("[Error]Ioctl operation");
        close(fd);
        return -1;
    }

    /* 绑定物理网卡 */
    stLocal.sll_family = AF_PACKET;
    stLocal.sll_ifindex = stIf.ifr_ifindex;
    stLocal.sll_protocol = htons(ETH_P_ALL);
    iRet = bind(fd, (struct sockaddr *)&stLocal, sizeof(stLocal));
    if (0 > iRet) {
        perror("[Error]Bind the interface");
        close(fd);
        return -1;
    }

    return fd;
}

typedef struct _fd_t {
    int fds;
    int fdr;
} fd_t;

static void *thread_recv(void *arg)
{
    fd_t *p = NULL;
    p = (fd_t *) arg;
    printf("rfd is %d, sfd is %d.\n", p->fdr, p->fds);
    int iRet = -1;
    socklen_t stFromLen = 0;
    char g_acRecvBuf[RCV_BUF_SIZE] = { 0 };
    int counter = 0;
    struct sockaddr addr = { 0 };

    stFromLen = sizeof(struct sockaddr);
    /* 循环监听 */
    while (1) {
        memset(g_acRecvBuf, 0, RCV_BUF_SIZE);

        /* 接收数据帧 */
        iRet = recvfrom(p->fdr, g_acRecvBuf, g_iRecvBufSize, 0, &addr, &stFromLen);
        if (0 > iRet) {
            continue;
        }
        if (addr.sll_pkttype == PACKET_OUTGOING) {
            continue;
        }
        /*
        if (0 == strncmp(g_acRecvBuf, "embedwayembedway", 16)) {
            //printf("recv send content, counter: %d....\n", counter);
            //break;
        }
        */
        strcpy(g_acRecvBuf, "embedwayembedway");
        //sendto(p->fds, g_acRecvBuf, iRet, 0, (struct sockaddr *)NULL, sizeof(struct sockaddr_ll));
        counter++;
        if (0 == (counter % 10000)) {
            printf("aaaaaaaaaaaaaa recv counter: len: %d, fd: %d, counter: %d.\n", iRet, p->fdr, counter);
        }

        /* 解析数据帧 */
        //ethdump_parseFrame(g_acRecvBuf);
    }
    return 0;
}

static void *thread_send(void *arg)
{
    fd_t *p = NULL;
    p = (fd_t *) arg;
    printf("rfd is %d, sfd is %d.\n", p->fdr, p->fds);
    int iRet = 256;
    socklen_t stFromLen = 0;
    char g_acRecvBuf[RCV_BUF_SIZE] = { 0 };
    int counter = 0;

    usleep(1000000);
    while (counter < 10000) {
        memset(g_acRecvBuf, 0, RCV_BUF_SIZE);
        strcpy(g_acRecvBuf, "abcdefghijklmnopqrstuvwxyz1234567890");

        sendto(p->fdr, g_acRecvBuf, iRet, 0, (struct sockaddr *)NULL, sizeof(struct sockaddr_ll));
        sendto(p->fds, g_acRecvBuf, iRet, 0, (struct sockaddr *)NULL, sizeof(struct sockaddr_ll));
        counter++;
        usleep(100);
        //printf("send counter: %d.\n", counter);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    pthread_t tid1;
    pthread_t tid2;
    fd_t fd1;
    fd_t fd2;

    /* 初始化SOCKET */
    fd1.fdr = ethdump_initSocket("cxxu1");
    if (0 > fd1.fdr) {
        return -1;
    }
    fd1.fds = ethdump_initSocket("cxxu1");
    if (0 > fd1.fds) {
        return -1;
    }
#if 1
    fd2.fdr = ethdump_initSocket("cxxu1");
    if (0 > fd2.fdr) {
        return -1;
    }
    fd2.fds = ethdump_initSocket("cxxu1");
    if (0 > fd2.fds) {
        return -1;
    }
#endif
    /* 捕获数据包 */
    pthread_create(&tid2, NULL, thread_recv, (void *)&fd2);
    pthread_create(&tid1, NULL, thread_send, (void *)&fd1);
    while (1);

    /* 关闭SOCKET */
    close(fd1.fds);
    close(fd1.fdr);
    close(fd2.fds);
    close(fd2.fdr);

    return 0;
}
