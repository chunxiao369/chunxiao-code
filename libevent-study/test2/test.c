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

int debug_print = 1;
int counter = 0;

#define DEBUG_PRINT(x, format,...) do { if(x){printf(format, ##__VA_ARGS__);}} while(0)
void read_and_write(int fd, short event, void *arg)
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
		//DEBUG_PRINT(debug_print, "fd: %d -- recv from client:\n", fd);
        counter++;
    } else {
        event_del((struct event*)arg);
        free(arg);
        shutdown(fd, SHUT_RDWR);
        return;
    }
    if ((counter % 10) == 0) {
		DEBUG_PRINT(debug_print, "counter: %d\n", counter);
    }
#if 0
    //bm_msg_process(buf_r, buf_s, &send_len);
    /* x86 doc set 4 bytes aligned */
    if (send_len % 4) {
        pad = 4 - (send_len % 4);
        send_len += pad;
    }

    DEBUG_PRINT(debug_print, "send to client, send_len: %d:\n", send_len);

    len = write(fd, buf_s, send_len);
    if (len < 0) {
        printf("error...............\n");
    }
#endif
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

extern int ethdump_initSocket(char *if_name);
int main(int argc, char *argv[])
{
	int s = 0;
    struct event ev;
    struct sigaction act;

    //SIGPIPE process
    act.sa_handler = SIG_IGN;
    if (sigaction(SIGPIPE, &act, NULL) != 0) {
        DEBUG_PRINT(debug_print, "SIGPIPE sigaction failed\n");
        exit(-1);
    }

    if (argc < 2) {
        DEBUG_PRINT(debug_print, "interface name\n");
        exit(-1);
    }
    s = ethdump_initSocket(argv[1]);
  
    /* Initial libevent. */
    event_init();
    /* Create event. */
    event_set(&ev, s, EV_READ | EV_PERSIST, read_and_write, &ev);
    /* Add event. */
    event_add(&ev, NULL);

    event_dispatch();
    return 0;
}

