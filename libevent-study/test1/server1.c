/* * libevent echo server example. */
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <event.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>

#define SERVER_PORT 4811
int connect_counter = 0;
int finish_counter = 0;
/** * 这个结构是指定的客户端数据，这个例子中只指定了读事件对象*/
struct client {
    struct event ev_read;
    struct sockaddr_in client_addr;
};     /** * 将一个socket设置成非阻塞模式 */
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

/** * 这个函数当客户端的socket可读时由libevent调用 */

void on_read(int fd, short ev, void *arg)
{
    struct client *client = (struct client *)arg;
    u_char buf[1024];
    int len, wlen;
    len = read(fd, buf, sizeof(buf));
    if (len == 0) {
        /* 客户端断开连接，在这里移除读事件并且释放客户数据结构 */
        finish_counter++;
        //printf("Client disconnected, finish_counter: %d.\n", finish_counter);
        close(fd);
        event_del(&client->ev_read);
        free(client);
        return;
    } else if (len < 0) {
        /* 出现了其它的错误，在这里关闭socket，移除事件并且释放客户数据结构 */
        printf("Socket failure, disconnecting client: %s", strerror(errno));
        close(fd);
        event_del(&client->ev_read);
        free(client);
        return;
    }
    if (strncmp((char *)buf, "counter", 7) == 0) {
        printf("recv counter: %d.\n", connect_counter);
        printf("send counter: %d.\n", finish_counter);
    }
    //printf("before read from %s.\n", inet_ntoa(client->client_addr.sin_addr));
    /* 为了简便，我们直接将数据写回到客户端。通常我们不能在非阻塞的应用程序中这么做，
    * 我们应该将数据放到队列中，等待可写事件的时候再写回客户端。
    */
    wlen = write(fd, buf, len);
    if (wlen < len) {
        /* 我们没有把所有数据都写回。如果我们有适当的队列/缓存机制，
         * 我们能够在再次的可写事件中再次写入剩余的数据。因为这是一个简单的例子，
         * 我们仅仅舍去了没有写入的数据。
         */
        printf("Short write, not all data echoed back to client.\n");
    }
}

/** * 当有一个连接请求准备被接受时，这个函数将被libevent调用。 */

void on_accept(int fd, short ev, void *arg)
{
    int client_fd;
    socklen_t client_len = sizeof(struct sockaddr_in);
    struct client *client;
    /* 我们接受了一个新的客户，分配一个新的客户数据结构对象来保存这个客户的状态。 */
    client = calloc(1, sizeof(*client));
    if (client == NULL)
        err(1, "malloc failed");
    /* 接受新的连接 */
    client_fd = accept(fd, (struct sockaddr *)&(client->client_addr), &client_len);
    if (client_fd == -1) {
        warn("accept failed");
        return;
    }
    /* 设置客户端socket为非阻塞模式。 */
    if (setnonblock(client_fd) < 0)
        warn("failed to set client socket non-blocking");
    /* 设置读事件，libevent将在客户端socket可读时调用on_read函数。
    * 我们也会我们也会不断的响应读事件，所以我们不用在每次读取时再次添加读事件。
    */
    event_set(&client->ev_read, client_fd, EV_READ | EV_PERSIST, on_read, client);
    /* 设置的事件并没有激活，使用添加事件让其激活。 */
    event_add(&client->ev_read, NULL);
    connect_counter++;
    //printf("Accepted connection from %s, counter: %d\n", inet_ntoa(client->client_addr.sin_addr), connect_counter);
}

int main(int argc, char **argv)
{
    int listen_fd;
    struct sockaddr_in listen_addr;
    int reuseaddr_on = 1;
    /* 接受连接请求的事件对象。 */
    struct event ev_accept;
    /* 初始化 libevent. */
    event_init();
    /* 创建我们监听的socket。 */
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
        err(1, "listen failed");
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on, sizeof(reuseaddr_on)) == -1)
        err(1, "setsockopt failed");
    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = INADDR_ANY;
    listen_addr.sin_port = htons(SERVER_PORT);
    if (bind(listen_fd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0)
        err(1, "bind failed");
    if (listen(listen_fd, 5) < 0)
        err(1, "listen failed");
    /* 设置socket为非阻塞模式，使用libevent编程这是必不可少的。 */
    if (setnonblock(listen_fd) < 0)
        err(1, "failed to set server socket to non-blocking");
    /* 我们现在有了一个监听的socket，我们创建一个读事件，当有客户连接时，接收通知。 */
    event_set(&ev_accept, listen_fd, EV_READ | EV_PERSIST, on_accept, NULL);
    event_add(&ev_accept, NULL);    /* 开始 libevent 的事件循环。 */
    event_dispatch();
    return 0;
}
