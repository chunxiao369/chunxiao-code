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
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <signal.h>

#define BM_PORT       8888
#define BM_MAX_SIZE   1024 

int debug_print = 0;

#define DEBUG_PRINT(x, format,...) do { if(x){printf(format, ##__VA_ARGS__);}} while(0)
void connection_time(int fd, short event, struct event *arg)
{
    uint8_t buf_r[BM_MAX_SIZE];
    uint8_t buf_s[BM_MAX_SIZE];
	int send_len = 0;
    int len;
    int pad;

    memset(buf_r, 0, BM_MAX_SIZE);
    memset(buf_s, 0, BM_MAX_SIZE);
    len = read(fd, buf_r, BM_MAX_SIZE);
    if (len > 0) {
		DEBUG_PRINT(debug_print, "fd: %d -- recv from client:\n", fd);
		DEBUG_PRINT(debug_print, "----------------------------");
		DEBUG_PRINT(debug_print, "\n");
    } else {
        event_del((struct event*)arg);
        free(arg);
        shutdown(fd, SHUT_RDWR);
        return;
    }
#if 0
    struct tm t;
    time_t now;
    time(&now);
    localtime_r(&now, &t);
    asctime_r(&t, buf_s);
    printf("send to client\n");
#endif
    //bm_msg_process(buf_r, buf_s, &send_len);
    /* x86 doc set 4 bytes aligned */
    if (send_len % 4) {
        pad = 4 - (send_len % 4);
        send_len += pad;
    }

    DEBUG_PRINT(debug_print, "send to client, send_len: %d:\n", send_len);
    DEBUG_PRINT(debug_print, "------------------------------------");
    DEBUG_PRINT(debug_print, "\n\n\n");

    len = write(fd, buf_s, send_len);
    if (len < 0) {
        printf("error...............\n");
    }
}

void connection_accept(int fd, short event, void *arg)
{
    const char on = 1;
    /* Accept a new connection. */
    struct sockaddr_in s_in;
    socklen_t len = sizeof(s_in);
    int ns = accept(fd, (struct sockaddr *)&s_in, &len);
    if (ns < 0) {
        perror("accept");
        return;
    }

    /* Install time server. */
    struct event *ev = malloc(sizeof(struct event));
    setsockopt(ns, IPPROTO_TCP, TCP_NODELAY, (void *)&on, sizeof(on));
    event_set(ev, ns, EV_WRITE|EV_PERSIST, (void *)connection_time, ev);
    event_add(ev, NULL);
    /* for debugging */
    fprintf(stderr, "%s(): new fd = %d, event = %p.\n", __func__, ns, ev);

}

int setnonblock(int fd)
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

int main(int argc, char *argv[])
{
	int ch;
    struct event ev;
    struct sockaddr_in s_in;
    struct sigaction act;

    //SIGPIPE process
    act.sa_handler = SIG_IGN;
    if (sigaction(SIGPIPE, &act, NULL) != 0) {
        DEBUG_PRINT(debug_print, "SIGPIPE sigaction failed\n");
        exit(-1);
    }

    /* Request socket. */
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket");
        exit(1);
    }
    /* bind() */
    bzero(&s_in, sizeof(s_in));
    s_in.sin_family = AF_INET;
    s_in.sin_port = htons(BM_PORT);
    s_in.sin_addr.s_addr = INADDR_ANY;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (void *)&ch, sizeof(ch)) < 0) {
        perror("setsockopt");
        exit(1);
    }
    if (bind(s, (struct sockaddr *)&s_in, sizeof(s_in)) < 0) {
        perror("bind");
        exit(1);
    }

    /* listen() set backlog 5 */
    if (listen(s, 5) < 0) {
        perror("listen");
        exit(1);
    }
    if (setnonblock(s) < 0) {
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

