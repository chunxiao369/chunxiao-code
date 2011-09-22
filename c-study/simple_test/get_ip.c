#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

int main(int argc, char *argv[])
{
    int i = 0;
    int sockfd;
    struct ifconf ifconf;
    char buf[512];
    struct ifreq *ifreq;
    struct ifreq ifr;
/*
    int a = 0x12345678;
    int b = 0x87654321;

    char c[32] = {0};

    sprintf(c, "%08x", a);
    char *p = c + 8;
    
    sprintf(p, "%08x", b);
    printf("%s.\n", c);

    char string[8] = {0};
    strncpy(string, c, 8);
    uint32_t d = (int)strtoul(string, NULL, 16);
    memset(string, 0, 8);
    strncpy(string, p, 8);
    //for (i = 0; i < 8; i++)
    //    printf("%c ", string[i]);
    uint32_t e = (int)strtoul(string, NULL, 16);

    printf("%08x, %08x.\n", d, e);
    return 0;
    */
    /*
    ifconf.ifc_len = 0;
    ifconf.ifc_ifcu.ifcu_buf = (caddr_t)NULL;
    */
    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }

    ioctl(sockfd, SIOCGIFCONF, &ifconf);   
    
    printf("ifconf.ifc_len: %d.\n", ifconf.ifc_len);
    ifreq = (struct ifreq*)buf;
    for(i = (ifconf.ifc_len / sizeof(struct ifreq)); i > 0; i--)
    {
        memset(&ifr, 0, sizeof(ifr));
        strcpy (ifr.ifr_name, ifreq->ifr_name);
        if (ioctl (sockfd, SIOCGIFFLAGS, &ifr) < 0)
            printf("Can't get interface flags for %s: %m", ifr.ifr_name);

        printf("name = [%s]\n", ifreq->ifr_name);
        printf("local addr = [%s]\n",
        inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));
        ifreq++;
    }
    return 0;
}
