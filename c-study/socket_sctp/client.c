#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#define MAX_BUFFER 1024
#define MY_PORT_NUM 33333       /* This can be changed to suit the need and should be same in server and client */

#define MAX_BIND_RETRYS 10
struct sockaddr_storage s_loc;
int l_len = 0;
int bind_r(int sk, struct sockaddr_storage *saddr)
{
    int error = 0, i = 0;
    char *host_s, *serv_s;

    if ((host_s = malloc(NI_MAXHOST)) == NULL) {
        fprintf(stderr, "\n\t\t*** host_s malloc failed!!! ***\n");
        exit(1);
    }
    if ((serv_s = malloc(NI_MAXSERV)) == NULL) {
        fprintf(stderr, "\n\t\t*** serv_s malloc failed!!! ***\n");
        exit(1);
    }

    do {
        if (i > 0)
            sleep(1);           /* sleep a while before new try... */

        error = getnameinfo((struct sockaddr *)saddr, l_len, host_s, NI_MAXHOST, serv_s, NI_MAXSERV, NI_NUMERICHOST);

        if (error)
            printf("%s\n", gai_strerror(error));

        printf("\tbind(sk=%d, [a:%s,p:%s])  --  attempt %d/%d\n", sk, host_s, serv_s, i + 1, MAX_BIND_RETRYS);

        error = bind(sk, (struct sockaddr *)saddr, l_len);

        if (error != 0) {
            if (errno != EADDRINUSE) {
                fprintf(stderr, "\n\n\t\t***bind: can "
                        "not bind to %s:%s: %s ****\n", host_s, serv_s, strerror(errno));
                exit(1);
            }
        }
        i++;
        if (i >= MAX_BIND_RETRYS) {
            fprintf(stderr, "Maximum bind() attempts. " "Die now...\n\n");
            exit(1);
        }
    } while (error < 0 && i < MAX_BIND_RETRYS);

    return 0;
}

int main(int argc, char *argv[])
{
    int connSock, ret;
    int in;
    //int flags;
    struct sockaddr_in servaddr;
    struct sockaddr_in *t_addr;
    //struct sctp_sndrcvinfo sndrcvinfo;

    //struct sctp_initmsg initmsg;
    //struct sctp_event_subscribe events;

    char buffer[MAX_BUFFER + 1] = { 0 };
    int datalen = 0;
    int i = 0;
    //char *p;
    struct addrinfo *res;
    int error = 0;
    char *host_s, *serv_s;

    if ((host_s = malloc(NI_MAXHOST)) == NULL) {
        fprintf(stderr, "\n\t\t*** host_s malloc failed!!! ***\n");
        exit(1);
    }
    if ((serv_s = malloc(NI_MAXSERV)) == NULL) {
        fprintf(stderr, "\n\t\t*** serv_s malloc failed!!! ***\n");
        exit(1);
    }

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
    //servaddr.sin_addr.s_addr = inet_addr("10.17.1.171");
    servaddr.sin_addr.s_addr = inet_addr("172.16.33.171");
    //servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    error = getaddrinfo("172.16.33.222", 0, NULL, &res);
    if (error) {
        printf("%s.\n", gai_strerror(error));
        exit(1);
    }

    switch (res->ai_family) {
    case AF_INET:
        t_addr = (struct sockaddr_in *)&s_loc;
        memcpy(t_addr, res->ai_addr, res->ai_addrlen);
        t_addr->sin_family = res->ai_family;
        //t_addr->sin_port = htons(local_port);

        l_len = res->ai_addrlen;
        break;
    default:
        break;
    }
    /*
       error = getnameinfo((struct sockaddr *)&s_loc, l_len, host_s,
       NI_MAXHOST, serv_s, NI_MAXSERV, NI_NUMERICHOST);
     */
    bind_r(connSock, &s_loc);
    ret = connect(connSock, (struct sockaddr *)&servaddr, sizeof(servaddr));

    if (ret == -1) {
        printf("Connection failed\n");
        perror("connect()");
        close(connSock);
        exit(1);
    }
    while (1) {
        if (i & 0x1) {
            strcpy(buffer, "1111111111");
        } else {
            strcpy(buffer, "2222222222");
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
        /*Recv welcome message */
        if ((in = recv(connSock, buffer, MAX_BUFFER, 0)) == -1) {
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
        usleep(1000000);
        i++;
    }
    close(connSock);

    return 0;
}
