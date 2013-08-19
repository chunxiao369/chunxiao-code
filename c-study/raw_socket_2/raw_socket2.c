#include <sys/types.h>          // socket
#include <sys/socket.h>         // socket
#include <sys/ioctl.h>          // ioctl
#include <net/if.h>             // ifreq
#include <string.h>             // strcpy
#include <stdio.h>              // printf
#include <linux/if_packet.h>    // sockaddr_ll
#include "raw_socket.h"
#define BOOL   unsigned char
#define U8     unsigned char
#define U16    unsigned short
#define TRUE   1
#define FALSE  0

static int fd;
static unsigned char if_mac[6];
static int if_index;
static struct sockaddr_ll link_a;

U8 gBuf[2000] = { 0 };

U8 gTxBuf[2000];
U16 gLen = 0;

BOOL InitEtherNetIf(void)
{
    struct ifreq req;

    if ((fd = socket(AF_PACKET, SOCK_RAW, htons(0x0003))) < 0) {
    //if ((fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
        printf("failed to create raw socket!\n");
        return FALSE;
    }

    strcpy(req.ifr_name, "oct0");
    if (ioctl(fd, SIOCGIFFLAGS, &req) < 0) {
        printf("failed to do ioctl!");
        return FALSE;
    }

    req.ifr_flags |= IFF_PROMISC;

    if (ioctl(fd, SIOCSIFFLAGS, &req) < 0) {
        printf("failed to set eth0 into promisc mode!");
        return FALSE;
    }

    if (ioctl(fd, SIOCGIFHWADDR, &req) < 0) {
        printf("failed to get IF hw address!");
        return FALSE;
    }

    memcpy(if_mac, req.ifr_hwaddr.sa_data, sizeof(if_mac));

    if (ioctl(fd, SIOCGIFINDEX, &req) < 0) {
        printf("failed to get IF hw address!");
        return FALSE;
    }

    if_index = req.ifr_ifindex;
    return TRUE;
}

BOOL GetPacket(U8 * buf, U16 * len)
{
    int length = 0;
    socklen_t abc = sizeof(link_a);
    link_a.sll_ifindex = if_index;
    memcpy(link_a.sll_addr, buf, link_a.sll_halen);

    bind(fd, (struct sockaddr *)&link_a, sizeof(link_a));

    length = recvfrom(fd, buf, 2000, 0, (struct sockaddr *)&link_a, &abc);
    if (length < 0) {
        return 0;
    } else {
        *len = length;
        return 1;
    }
}

BOOL SendPacket(U8 * buf, U16 len)
{
    link_a.sll_ifindex = if_index;
    memcpy(link_a.sll_addr, buf, link_a.sll_halen);

    if (sendto(fd, buf, len, 0, (struct sockaddr *)&link_a, sizeof(link_a)) < 0) {
        printf("failed to send to RAW socket!\r\n");
        return 0;
    }
    return 1;
}

BOOL GetMacAddress(U8 * mac)
{
    memcpy(mac, if_mac, sizeof(if_mac));
    return TRUE;
}

void DispalyBuf(U8 * buf, U16 size)
{
    int i;
    for (i = 0; i < size; i++) {
        if ((0 == i % 16) && (i > 0))
            printf("\n");
        printf("%02x ", buf[i]);
    }
    printf("\n");
}

int main(void)
{
    BOOL ret;

    for (ret = 0; ret < 64; ret++)
        gTxBuf[ret] = ret;

    if (InitEtherNetIf()) {
        printf("send:\n");
        DispalyBuf(gTxBuf, 64);
        while (1) {
            sleep(1);
            SendPacket(gTxBuf, 64);
        }
        /*
        while (1) {
            if (GetPacket(gBuf, &gLen))
                DispalyBuf(gBuf, gLen);
        }
        */
    }
    return 0;
}
