#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>

#define PORT           1234
#define MAXDATASIZE    1024
 
main()
{
    int        sockfd;
    struct    sockaddr_in server;
    struct    sockaddr_in client;
    int        sin_size;
    int     num;
    char     msg[MAXDATASIZE];
    
    /*Create UDP socket*/
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        /*handle exception*/
        perror("Creating socket failed");
        exit(1);
    }
    
    bzero(&server, sizeof(server));
    server.sin_family        = AF_INET;
    server.sin_port            = htons(PORT);
    server.sin_addr.s_addr     = htonl(INADDR_ANY);
    if(bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        /*handle exception*/
        perror("Bind error");
        exit(1);
    }
    
    sin_size = sizeof(struct sockaddr_in);
    while(1)
    {
        /*recvfrom() get client's information, include the value of sin_size*/
        num = recvfrom(sockfd, msg, MAXDATASIZE, 0,
                            (struct sockaddr *)&client, (socklen_t *)&sin_size);
        if(num < 0)
        {
            perror("recvfrom() error");
            exit(1);
        }
    
        msg[num] = '\0';
        /*Prints client's IP*/
        printf("You got a message \"%s\" from %s\n", msg, inet_ntoa(client.sin_addr));
        
        /*
         *由于client中存放的地址是刚被接收的客户, 因而信息回送给该客户
         *故本服务器可服务多个客户而不混乱
         *什么意思??????????????????????????
         */
        sendto(sockfd, "Welcome to my server.\n", 22, 0,
                (struct sockaddr *)&client, sin_size);
    
        /*if msg is "quit", then break the while*/
        if(!strcmp(msg, "quit"))
        {
            break;
        }
    }
        
    close(sockfd);
	exit(0);
}

 


