// client.c
// Compile with: gcc client.c -o client -levent -levent_openssl -lssl -lcrypto
// Run with: ./client IP PORT "msg"
// If msg is "quit", server will exit after receiving it.
// Client skips certificate verification for simplicity (self-signed cert).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/bufferevent_ssl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

struct info {
    char *msg;
    struct bufferevent *bev;
};

void read_cb(struct bufferevent *bev, void *arg) {
    struct info *inf = arg;
    struct evbuffer *input = bufferevent_get_input(bev);
    char buf[1024];
    size_t n_read = evbuffer_remove(input, buf, sizeof(buf));
    if (n_read > 0) {
        buf[n_read] = '\0';
        printf("Server response: %s\n", buf);
    }
    // After reading response, exit
    event_base_loopbreak(bufferevent_get_base(bev));
}

void event_cb(struct bufferevent *bev, short events, void *arg) {
    struct info *inf = arg;
    if (events & BEV_EVENT_CONNECTED) {
        printf("Connected to server.\n");
        bufferevent_write(bev, inf->msg, strlen(inf->msg));
    } else if (events & BEV_EVENT_ERROR) {
        perror("Connection error");
        event_base_loopbreak(bufferevent_get_base(bev));
    } else if (events & BEV_EVENT_EOF) {
        event_base_loopbreak(bufferevent_get_base(bev));
    }
}

void init_openssl() {
    SSL_load_error_strings();
    SSL_library_init();
}

SSL_CTX *create_ssl_ctx() {
    SSL_CTX *ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    // Skip verification for self-signed cert
    SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);
    return ctx;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s IP PORT \"msg\"\n", argv[0]);
        return 1;
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    char *msg = argv[3];

    init_openssl();
    SSL_CTX *ctx = create_ssl_ctx();

    struct event_base *base = event_base_new();
    if (!base) {
        fprintf(stderr, "Couldn't create event_base\n");
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    inet_pton(AF_INET, ip, &sin.sin_addr);

    SSL *ssl = SSL_new(ctx);
    struct bufferevent *bev = bufferevent_openssl_socket_new(base, -1, ssl,
                                                             BUFFEREVENT_SSL_CONNECTING,
                                                             BEV_OPT_CLOSE_ON_FREE);

    struct info *inf = malloc(sizeof(struct info));
    inf->msg = msg;
    inf->bev = bev;

    bufferevent_setcb(bev, read_cb, NULL, event_cb, inf);
    bufferevent_enable(bev, EV_READ | EV_WRITE);

    if (bufferevent_socket_connect(bev, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        bufferevent_free(bev);
        fprintf(stderr, "Connection failed\n");
        return 1;
    }

    event_base_dispatch(base);

    bufferevent_free(bev);
    free(inf);
    SSL_CTX_free(ctx);
    event_base_free(base);

    return 0;
}
