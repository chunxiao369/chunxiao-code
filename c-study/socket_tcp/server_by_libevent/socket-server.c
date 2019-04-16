
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <event.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

static int g_count = 0;

static int __sal_setnonblock(int fd)
{
    int flags;
    flags = fcntl(fd, F_GETFL);
    if (flags < 0)
        return flags;
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0)
        return -1;
    return 0;
}

void connection_time(int fd, short event, struct event *arg)
{
    char buf[1024];
    struct tm t;
    time_t now;
    int len;

    memset(buf, 0, 1024);
    len = read(fd, buf, 1024);
    if (len > 0) {
    } else {
        //printf("fd: %d, len: %d, buf: %s\n", fd, len, buf);
        return;
#if 0
        /* use special message to close the socket */
        event_del((struct event*)arg);
        free(arg);
        shutdown(fd, SHUT_RDWR);
        return;
#endif
    }
    time(&now);
    localtime_r(&now, &t);
    asctime_r(&t, buf);
    //printf("send to client\n");
    len = write(fd, buf, strlen(buf));
    if (len < 0) {
        printf("error...............\n");
    }
    //printf("line: %d.\n", __LINE__);
}

void connection_accept(int fd, short event, void *arg)
{
    /* Accept a new connection. */
    struct sockaddr_in s_in;
    socklen_t len = sizeof(s_in);
    int ns = accept(fd, (struct sockaddr *)&s_in, &len);
    if (ns < 0) {
        perror("accept");
        return;
    }
    g_count++;
    if (__sal_setnonblock(ns) < 0) {
        printf("failed to set client socket non-blocking\n");
        return;
    }
    /* Install time server. */
    struct event *ev = malloc(sizeof(struct event));
    event_set(ev, ns, EV_WRITE|EV_PERSIST, (void *)connection_time, ev);
    event_add(ev, NULL);
    /* for debugging */
    printf("%s(): new fd = %d, from: %s, event = %p.\n", __func__, ns, inet_ntoa(s_in.sin_addr), ev);
}

static void __sal_prg_exit(void)
{
    /* free all memory */
    exit(0);
    return;
}

static void __sal_on_signal(int signo)
{
    printf("client counter = %u\n", g_count);
    __sal_prg_exit();
    return;
}

int main(void)
{
    struct event ev;
    struct sockaddr_in s_in;

    signal(SIGHUP, __sal_on_signal);
    signal(SIGINT, __sal_on_signal);

    /* Request socket. */
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket");
        exit(1);
    }

    /* bind() */
    bzero(&s_in, sizeof(s_in));
    s_in.sin_family = AF_INET;
    s_in.sin_port = htons(8888);
    s_in.sin_addr.s_addr = INADDR_ANY;
    if (bind(s, (struct sockaddr *)&s_in, sizeof(s_in)) < 0) {
        perror("bind");
        exit(1);
    }

    /* listen() */
    if (listen(s, 5) < 0) {
        perror("listen");
        exit(1);
    }
    /* 设置socket为非阻塞模式，使用libevent编程这是必不可少的。 */
    if (__sal_setnonblock(s) < 0) {
        perror("setnonblock");
        exit(1);
    }

    /* Initial libevent. */
    event_init();
    /* Create event. */
    event_set(&ev, s, EV_READ | EV_PERSIST, connection_accept, &ev);
    /* Add event. */
    event_add(&ev, NULL);

    event_dispatch();
    return 0;
}
