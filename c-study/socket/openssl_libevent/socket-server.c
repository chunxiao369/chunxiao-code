// server.c
// Compile with: gcc server.c -o server -levent -levent_openssl -lssl -lcrypto
// Run with: ./server PORT
// Assumes server.crt and server.key exist (self-signed cert/key).
// Generate self-signed cert: openssl req -x509 -newkey rsa:2048 -keyout server.key -out server.crt -days 365 -nodes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/bufferevent_ssl.h>
#include <event2/listener.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define CERT_FILE "server.crt"
#define KEY_FILE "server.key"

struct event_base *base;
SSL_CTX *ctx;

void read_cb(struct bufferevent *bev, void *arg) {
    struct evbuffer *input = bufferevent_get_input(bev);
    char buf[1024];
    char *buf_send = "Welcome, server by openssl libevent.\n";
    size_t n_read;

    while ((n_read = evbuffer_remove(input, buf, sizeof(buf))) > 0) {
        buf[n_read] = '\0';
        printf("Received: %s\n", buf);

        if (strstr(buf, "quit") != NULL) {
            printf("Received quit command. Shutting down server.\n");
            event_base_loopbreak(base);
            return;
        }

        // Echo back for interaction (optional, but shows interaction)
        //bufferevent_write(bev, buf, n_read);
        bufferevent_write(bev, buf_send, strlen(buf_send));
    }
}

void event_cb(struct bufferevent *bev, short events, void *arg) {
    if (events & BEV_EVENT_ERROR) {
        perror("Connection error");
    }
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        bufferevent_free(bev);
    }
}

void accept_cb(struct evconnlistener *listener, evutil_socket_t fd,
               struct sockaddr *addr, int socklen, void *arg) {
    SSL *ssl = SSL_new(ctx);
    struct bufferevent *bev = bufferevent_openssl_socket_new(base, fd, ssl,
                                                             BUFFEREVENT_SSL_ACCEPTING,
                                                             BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, read_cb, NULL, event_cb, NULL);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void init_openssl() {
    SSL_load_error_strings();
    SSL_library_init();
}

SSL_CTX *create_ssl_ctx() {
    SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    if (SSL_CTX_use_certificate_file(ctx, CERT_FILE, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, KEY_FILE, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    return ctx;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s PORT\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);

    init_openssl();
    ctx = create_ssl_ctx();

    base = event_base_new();
    if (!base) {
        fprintf(stderr, "Couldn't create event_base\n");
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    struct evconnlistener *listener = evconnlistener_new_bind(base, accept_cb, NULL,
                                                              LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
                                                              -1, (struct sockaddr*)&sin, sizeof(sin));
    if (!listener) {
        fprintf(stderr, "Couldn't create listener\n");
        return 1;
    }

    event_base_dispatch(base);

    evconnlistener_free(listener);
    SSL_CTX_free(ctx);
    event_base_free(base);

    return 0;
}
