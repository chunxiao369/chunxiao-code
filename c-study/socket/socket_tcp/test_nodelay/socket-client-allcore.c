
/*
 * TCP客户端
 */
#define _GNU_SOURCE
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

#define PORT           8888
#define MAXDATASIZE    1024

char ip_str[256] = {0};
void *myfun(void *arg)
{
    int fd;
    int numbytes;
    char buf[MAXDATASIZE];
    struct sockaddr_in server;  //server's address information
    int i = 0;
    int on = 1;
    int ch;
    int nNetTimeout = 0;

    /*Create TCP socket */
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    //if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        /*handle exception */
        perror("Creating socket failed");
        exit(1);
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    /*下面一句有必要做网络字节序到主机字节序的转化？ */
//    server.sin_addr = *((struct in_addr *)he->h_addr);
    server.sin_addr.s_addr = inet_addr(ip_str);

#if 1
    if (connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
        /*handle exception */
        perror("connect() error\n");
        exit(1);
    }
    //fcntl(fd, F_SETFL, O_NONBLOCK);
    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
    /*Recv welcome message */
    /*
       if((numbytes = recv(fd, buf, MAXDATASIZE, 0)) == -1)
       {
       perror("recv() error\n");
       exit(1);
       }
     */
    setsockopt (fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof (on));
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *)&ch, sizeof(ch)) < 0) {
        perror("setsockopt");
        exit(1);
    }
#endif
    for (i = 0; i < 1024; i++) {
        buf[i] = 'a';
    }
    while (1) {
        buf[0] = 'b';
#if 0
        if (write(fd, buf, 1024) < 0) {
            printf("error...............\n");
            break;
        }
#endif
#if 1
        if (sendto(fd, buf, 1024, 0, (struct sockaddr *)&server, sizeof(server)) < 0) {
            break;
        }
#endif
        //memset(buf, 0, 1024);
        i++;
    }
    sendto(fd, "send file abc", strlen("send file abc"), 0, (struct sockaddr *)&server, sizeof(server));
    sendto(fd, "send file over", strlen("send file over"), 0, (struct sockaddr *)&server, sizeof(server));

    buf[numbytes] = '\0';
    //printf("Server Message: %s\n", buf);

    close(fd);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int i;
    cpu_set_t mask;
    struct hostent *he;
    pthread_t tid[64];
    uint64_t core_mask = (~0ul);
    int num = sysconf(_SC_NPROCESSORS_CONF);

    if (argc != 3) {
        printf("Usage: %s <IP Address> coremask\n", argv[0]);
        exit(1);
    }
    if ((he = gethostbyname(argv[1])) == NULL) {
        printf("gethostbyname() error\n");
        exit(1);
    }
    strcpy(ip_str, argv[1]);

    if (argc >= 3) {
        core_mask = strtoul(argv[2], NULL, 16);
    }
    printf("system has %d processor(s), use 0x%lx\n", num, core_mask);
    for (i = 0; i < num; i++) {
        if ((core_mask & (1ul << i)) == 0ul) {
           continue;
        }
        if (pthread_create(&tid[i], NULL, (void *)myfun, NULL) != 0) {
            fprintf(stderr, "thread create failed\n");
            return -1;
        }
    }
    for (i = 0; i < num; i++) {
        if ((core_mask & (1ul << i)) == 0ul) {
           continue;
        }
        CPU_ZERO(&mask);
        CPU_SET(i, &mask);
        if (pthread_setaffinity_np(tid[i], sizeof(mask), &mask) < 0) {
            fprintf(stderr, "set thread affinity failed\n");
        }
    }
    for (i = 0; i < num; i++) {
        if ((core_mask & (1ul << i)) == 0ul) {
           continue;
        }
        pthread_join(tid[i], NULL);
    }
    return 0;
}
