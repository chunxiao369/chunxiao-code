#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#define BUF_SIZE 30

pthread_mutex_t mt;

void *clnt_handling(void *param)
{
    int str_len;
    int clnt_sock = *((int *)param);
    char buff[BUF_SIZE] = { 0 };
    char buff_s[BUF_SIZE] = { 0 };
    printf("thread create.....socket: %d\n", clnt_sock);
    //pthread_mutex_lock(&mt);
    while (1) {
        str_len = read(clnt_sock, buff, BUF_SIZE);
        if (str_len <= 0)
            break;
        str_len = sprintf(buff_s, "%s+%d", buff, clnt_sock);
        write(clnt_sock, buff_s, str_len);
    }
    //pthread_mutex_unlock(&mt);
}

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    int clnt_addr_sz;
    pthread_t tid;
    pthread_mutex_init(&mt, NULL);

    if (argc != 2) {
        printf("Usage %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(serv_sock, 5);
    while (1) {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_sz);
        pthread_create(&tid, NULL, clnt_handling, (void *)&clnt_sock);
        pthread_detach(tid);
    }

    pthread_mutex_destroy(&mt);
    close(serv_sock);

    return 0;
}
