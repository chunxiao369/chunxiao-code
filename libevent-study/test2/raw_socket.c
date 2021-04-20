#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <errno.h>

/* 接收缓冲区大小 */
#define RCV_BUF_SIZE     1024 * 5
static int g_iRecvBufSize = RCV_BUF_SIZE;

/* 物理网卡混杂模式属性操作 */
static int ethdump_setPromisc(const char *pcIfName, int fd, int iFlags)
{
    int iRet = -1;
    struct ifreq stIfr;

    /* 获取接口属性标志位 */
    strcpy(stIfr.ifr_name, pcIfName);
    iRet = ioctl(fd, SIOCGIFFLAGS, &stIfr);
    if (0 > iRet) {
        perror("[Error]Get Interface Flags");
        return -1;
    }

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

/* Init L2 Socket */
int ethdump_initSocket(char *if_name)
{
    int iRet = -1;
    int fd = -1;
    struct ifreq stIf;
    struct sockaddr_ll stLocal = { 0 };

    /* 创建SOCKET */
    fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (0 > fd) {
        perror("[Error]Initinate L2 raw socket");
        return -1;
    }

    /* 网卡混杂模式设置 */
    ethdump_setPromisc(if_name, fd, 1);

    /* 设置SOCKET选项 */
    iRet = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &g_iRecvBufSize, sizeof(int));
    if (0 > iRet) {
        perror("[Error]Set socket option");
        close(fd);
        return -1;
    }

    /* 获取物理网卡接口索引 */
    strcpy(stIf.ifr_name, if_name);
    iRet = ioctl(fd, SIOCGIFINDEX, &stIf);
    if (0 > iRet) {
        perror("[Error]Ioctl operation");
        close(fd);
        return -1;
    }

    /* 绑定物理网卡 */
    stLocal.sll_family = PF_PACKET;
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

