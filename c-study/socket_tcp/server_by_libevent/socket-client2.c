
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
#include <errno.h>

#define PORT           8888
#define MAXDATASIZE    100
#define MAXCONN        2

int *g_fd = NULL;
void *thread()
{
    for (i = 0; i < MAX; i++) {
        printf("thread2 : number = %d\n", number);
        number++;
        sleep(1);
    }
}

int main(int argc, char **argv)
{
    int numbytes;
    int i = 0;
    char buf[MAXDATASIZE];
    pthread_t thread[MAXCONN];
    struct sockaddr_in server[MAXCONN]; //server's address information
    struct hostent *he;

    if (argc != 2) {
        printf("Usage: %s <IP Address>\n", argv[0]);
        exit(1);
    }

    if ((he = gethostbyname(argv[1])) == NULL) {
        printf("gethostbyname() error\n");
        exit(1);
    }
    for (i = 0; i < MAXCONN; i++) {
        if ((temp = pthread_create(&thread[i], NULL, thread, NULL)) != 0) {
            printf("线程1创建失败!\n");
        } else {
            printf("线程1被创建\n");
        }
    }

    g_fd = malloc(sizeof(int) * MAXCONN);
    if (g_fd == NULL) {
        exit(1);
    }
    for (i = 0; i < MAXCONN; i++) {
        /*Create TCP socket */
        if ((g_fd[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
            /*handle exception */
            perror("Creating socket failed");
            exit(1);
        }

        bzero((void *)&server[i], sizeof(struct sockaddr_in));
        server[i].sin_family = AF_INET;
        /*下面一句有必要做网络字节序到主机字节序的转化？ */
        //server.sin_addr = *((struct in_addr *)he->h_addr);
        server[i].sin_addr.s_addr = inet_addr(argv[1]);
        server[i].sin_port = htons(PORT);

        if (connect(g_fd[i], (struct sockaddr *)&server[i], sizeof(struct sockaddr_in)) != 0) {
            /*handle exception */
            printf("errno: %d.\n", errno);
            exit(1);
        }
        printf("fd is %d.\n", g_fd[i]);
    }
    while (1) {
        //printf("i is %d, %d\n", i, __LINE__);
        /*send something */
        for (i = 0; i < MAXCONN; i++) {
            //printf("i is %d, %d, fd: %d\n", i, __LINE__, g_fd[i]);
            sendto(g_fd[i], "counter", strlen("counter"), 0, (struct sockaddr *)&server[i], sizeof(struct sockaddr));
            //printf("i is %d, %d\n", i, __LINE__);
            /*Recv welcome message */
            if ((numbytes = recv(g_fd[i], buf, MAXDATASIZE, 0)) == -1) {
                perror("recv() error\n");
                continue;
            }
            buf[numbytes] = '\0';
            //printf("Server Message: %s\n", buf);
        }
    }
    for (i = 0; i < MAXCONN; i++) {
        close(g_fd[i]);
    }
    if (thread[0] != 0) {       //comment4
        pthread_join(thread[0], NULL);
        printf("线程1已经结束\n");
    }
    if (thread[1] != 0) {       //comment5
        pthread_join(thread[1], NULL);
        printf("线程2已经结束\n");
    }

    if (g_fd != NULL) {
        free(g_fd);
    }
    return 0;
}
