
/*
 * TCP客户端
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#define PORT           1234
#define MAXDATASIZE    1024

int main(int argc, char **argv)
{
    int fd;
    int numbytes;
    char buf[MAXDATASIZE];
    struct sockaddr_in server;  //server's address information
    struct hostent *he;
    int i = 0;
    int on = 1;

    if (argc != 2) {
        printf("Usage: %s <IP Address>\n", argv[0]);
        exit(1);
    }

    if ((he = gethostbyname(argv[1])) == NULL) {
        printf("gethostbyname() error\n");
        exit(1);
    }

    /*Create TCP socket */
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        /*handle exception */
        perror("Creating socket failed");
        exit(1);
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    /*下面一句有必要做网络字节序到主机字节序的转化？ */
//    server.sin_addr = *((struct in_addr *)he->h_addr);
    server.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
        /*handle exception */
        perror("connect() error\n");
        exit(1);
    }

    /*Recv welcome message */
    /*
       if((numbytes = recv(fd, buf, MAXDATASIZE, 0)) == -1)
       {
       perror("recv() error\n");
       exit(1);
       }
     */
    //setsockopt (fd, SOL_TCP, TCP_NODELAY, &on, sizeof (on));
    for (i = 0; i < 1024; i++) {
        buf[i] = 'a';
    }
    while (i < 10) {
        usleep(1);
        buf[0] = 'a' + i;
        if (sendto(fd, buf, 1024, 0, (struct sockaddr *)&server, sizeof(server)) < 0) {
            break;
        }
        memset(buf, 0, 1024);
        i++;
    }
    sendto(fd, "send file abc", strlen("send file abc"), 0, (struct sockaddr *)&server, sizeof(server));
    sendto(fd, "send file over", strlen("send file over"), 0, (struct sockaddr *)&server, sizeof(server));

    buf[numbytes] = '\0';
    //printf("Server Message: %s\n", buf);

    close(fd);
    return 0;
}
