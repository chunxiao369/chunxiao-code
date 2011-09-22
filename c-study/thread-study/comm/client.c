#include  "header.h"

int main()
{
    int i, j;
    //pthread_t thread;
    int sockfd;
    char r_buffer[1024], w_buffer[1024];
    struct sockaddr_in server_addr;
    struct hostent *host;
    int portnumber, nread, nwrite;
    struct timeval start_tv, end_tv;

    //struct   timezone   start_tz,end_tz; 

    if ((host = gethostbyname("127.0.0.1")) == NULL) {
        printf("Gethostname   error.\n ");
        return 1;
    }

    portnumber = 4000;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Socket   error:%s\n ", strerror(errno));
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portnumber);
    server_addr.sin_addr = *((struct in_addr *) host->h_addr);

    /*如果连接成功，则将套接子设为非阻塞模式 */
    if (connect(sockfd, (struct sockaddr *) (&server_addr),
                sizeof(struct sockaddr)) == -1) {
        printf("Connect   error:%s\n ", strerror(errno));
    } else
        fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK);

    i = 0;
    gettimeofday(&start_tv, NULL);
    for (;;) {
        memset(w_buffer, 0, sizeof(w_buffer));
        for (j = 0; j < 10; j++) {
            memcpy((w_buffer + j * 10), "987654321* ", 10);
        }

        nwrite = write(sockfd, w_buffer, sizeof(w_buffer));
        if (nwrite > 0) {
            i++;
        }

        gettimeofday(&end_tv, NULL);
        //printf( "/*****/diff   time   is:%d   milliseconds\n ", 
        //             (end_tv.tv_sec-start_tv.tv_sec)*1000+(end_tv.tv_usec-start_tv.tv_usec)/1000); 
        if ((end_tv.tv_sec - start_tv.tv_sec) >= 1) {
            gettimeofday(&start_tv, NULL);
            printf("one   second   send   data   number:%d\n ", i);
            i = 0;
        }
        usleep(5000);

        bzero(r_buffer, 1024);
        nread = read(sockfd, r_buffer, 1024);
        if (nread > 0)
            printf("client   read   data   from   socket:%s\n\n ", r_buffer);
    }

    close(sockfd);
    return 0;
}

