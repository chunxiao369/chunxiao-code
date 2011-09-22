
#include   "header.h"

#define   TRUE               1
#define   FALSE             0
#define   MAX_SOCKETFD_NUM     5

int s_sockfd, network_flag;

unsigned int CreateSocket()
{
    struct sockaddr_in server_addr;
    int n;

    if ((s_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Socket   error:%s\n ", strerror(errno));
        return FALSE;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    n = 1;
    /*如果服务器终止后,服务器可以第二次快速启动而不用等待一段时间   */
    setsockopt(s_sockfd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(int));

    if (bind
        (s_sockfd, (struct sockaddr *) (&server_addr),
         sizeof(struct sockaddr)) == -1) {
        printf("Bind   error:%s\n ", strerror(errno));
        return FALSE;
    } else
        printf("Bind   ok.\n ");

    if (listen(s_sockfd, 5) == -1) {
        printf("Listen   error:%s\n ", strerror(errno));
        return FALSE;
    } else
        printf("Listen   ok.\n ");
    return TRUE;
}

void *NetThread(void *arg)
{
    int *retval;
    unsigned int fd, nread, nwrite, count;
    unsigned char buffer[1024], w_buffer[256];
    struct timeval start_tv, end_tv;

    //struct timezone start_tz, end_tz; 

    count = 0;
#ifdef   SEND_VALUE
    fd = *((int *) arg);
#else
    fd = (unsigned int) arg;
#endif
    gettimeofday(&start_tv, NULL);

    while (1) {
        nread = 0;
        memset(buffer, 0, sizeof(buffer));
        memset(w_buffer, 0, sizeof(w_buffer));

        nread = read(fd, buffer, 1024);
        if (nread > 0) {
            count++;
            //printf( "Server   receive:%s\n ",buffer); 
        }

        gettimeofday(&end_tv, NULL);
        if ((end_tv.tv_sec - start_tv.tv_sec) >= 1) {
            gettimeofday(&start_tv, NULL);
            memcpy(w_buffer, "test ", 4);
            if ((nwrite = write(fd, w_buffer, sizeof(w_buffer))) == -1) {
                printf("Write   error:%s\n ", strerror(errno));
                break;
            }
            printf("server   %d   receive   data   number:%d\n ",
                   (int)pthread_self(), count);
            count = 0;
        }

        usleep(1000);
    }
    pthread_exit(&retval);
}

void NetServer()
{
    //unsigned int retval;
    struct sockaddr_in client_addr;
    socklen_t sin_size;
    unsigned char client_ip[16];

#ifdef   SEND_VALUE
    unsigned int *new_fd;
#else
    unsigned int new_fd;
#endif
    pthread_t net_thread;

    while (1) {
        sin_size = sizeof(struct sockaddr_in);

#ifdef   SEND_VALUE
        if ((*new_fd =
             accept(s_sockfd, (struct sockaddr *) (&client_addr),
                    &sin_size)) == -1)
#else
        if ((new_fd =
             accept(s_sockfd, (struct sockaddr *) (&client_addr),
                    &sin_size)) == -1)
#endif
        {
            printf("Accept   error:%s\n ", strerror(errno));
            break;
        } else {
            strcpy((char *)client_ip, inet_ntoa(client_addr.sin_addr));
            printf("Server   get   connection   from:%s\n ", client_ip);

            /*   Create   process   net   data   thread 
             *   为线程传递更具移植性参数（即：将整数值传给被调用函数，值的拷贝被推入被调用函数的栈中） 
             *   传递指针参数时，多个线程同时存取一个共享变量（new_fd)而没有同步措施 
             */
#ifdef   SEND_VALUE
            if (pthread_create(&net_thread, NULL, NetThread, new_fd) != 0)
#else
            /*   传递指针参数 */
            if (pthread_create(&net_thread, NULL, NetThread, (void *) new_fd) !=
                0)
#endif
            {
                printf("Create   server   thread   failed.\n ");
            } else
                printf("Thread   server   created!\n ");
        }

        usleep(10000);
    }
}

void CheckConnect()
{
    network_flag = 0;
}

int main()
{
    int ret;

    signal(SIGPIPE, CheckConnect);

    ret = CreateSocket();
    if (ret == FALSE) {
        printf("Create   socket   faile.\n ");
        return FALSE;
    }

    NetServer();

    usleep(10000);
    return 0;
}

