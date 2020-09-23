#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#define MAX_BUFFER 1024
#define MY_PORT_NUM 62324       /* This can be changed to suit the need and should be same in server and client */

int main(int argc, char *argv[])
{
    int connSock, ret;
    int in;
    int flags;
    struct sockaddr_in servaddr;
    struct sctp_sndrcvinfo sndrcvinfo;
    char buffer[MAX_BUFFER + 1] = {0};
    int datalen = 0;
    int i = 0;
    char *p;

    /*Get the input from user */

    connSock = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

    if (connSock == -1) {
        printf("Socket creation failed\n");
        perror("socket()");
        exit(1);
    }

    bzero((void *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MY_PORT_NUM);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = connect(connSock, (struct sockaddr *)&servaddr, sizeof(servaddr));

    if (ret == -1) {
        printf("Connection failed\n");
        perror("connect()");
        close(connSock);
        exit(1);
    }
    while (1) {
        printf("Enter data to send: ");
        p = fgets(buffer, MAX_BUFFER, stdin);
        if (NULL == p) {
            return 1;
        }
        /* Clear the newline or carriage return from the end */
        buffer[strlen(buffer)] = 0;
        datalen = strlen(buffer);

        ret = sctp_sendmsg(connSock, (void *)buffer, datalen, NULL, 0, 0, 0, 0, 0, 0);
        if (ret == -1) {
            printf("Error in sctp_sendmsg\n");
            perror("sctp_sendmsg()");
            break;
        } else
            printf("Successfully sent %d bytes data to server\n", ret);
#if 1
        /*Recv welcome message*/
        if((in = recv(connSock, buffer, MAX_BUFFER, 0)) == -1) {
            perror("recv() error\n");
            break;
        } else {
            buffer[in] = '\0';
            printf("Server Message: %s\n", buffer);
        }
#endif
 
#if 0
        in = sctp_recvmsg(connSock, buffer, MAX_BUFFER, (struct sockaddr *)NULL, 0, &sndrcvinfo, &flags);

        if (in == -1) {
            printf("Error in sctp_recvmsg\n");
            perror("sctp_recvmsg()");
        } else {
            buffer[in] = '\0';
            printf(" Length of Data received: %d\n", in);
            printf(" Data : %s\n", (char *)buffer);
        }
#endif
        i++;
    }
    close(connSock);

    return 0;
}
