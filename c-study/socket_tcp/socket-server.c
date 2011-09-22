
/*
    创建套接字
    绑定套接字
    设置套接字为监听模式, 进入被动接受连接请求状态
    接受连接, 建立连接
    读写数据
    终止连接  
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT       1234
#define BACKLOG    1        //Number of allowed connections

int main()
{
    int        listenfd;
    int        connectfd;
    struct     sockaddr_in server;
    struct     sockaddr_in client;
    int        sin_size;
    
    /*Create TCP socket*/
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        /*handle exception*/
        perror("Creating socket failed");
        exit(1);
    }
    
    /*Set socket can be reused*/
    int opt = SO_REUSEADDR;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    bzero(&server, sizeof(server));
    server.sin_family    = AF_INET;
    server.sin_port    = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        /*handle exception*/
        perror("Bind error");
        exit(1);
    }
    
    if(listen(listenfd, BACKLOG) == -1)
    {
        /*handle exception*/
        perror("Listen error");
        exit(1);
    }
    
    sin_size = sizeof(struct sockaddr_in);
    connectfd = accept(listenfd, (struct sockaddr *)&client, (socklen_t *)&sin_size);
    if(connectfd == -1)
    {
        /*handle exception*/
        perror("Accept error");
        exit(1);
    }    
    
    /*prints client's IP*/
    printf("You got a connection from %s\n", inet_ntoa(client.sin_addr));
    
    /*Send welcome message*/
    send(connectfd, "Welcome to my server.\n", 22, 0);
    
    close(connectfd);
    close(listenfd);
    return 0;
}
 

