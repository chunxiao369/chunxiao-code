#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT           1234
#define MAXDATASIZE    1024

void openssl_init(void) {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void openssl_cleanup(void) {
    EVP_cleanup();
}

SSL_CTX *create_dtls_client_ctx(void) {
    const SSL_METHOD *method = DTLS_client_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        return NULL;
    }
#if 1
    if (!SSL_CTX_load_verify_locations(ctx, "server.crt", NULL)) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return NULL;
    }
#endif
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    //SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);
    SSL_CTX_set_min_proto_version(ctx, DTLS1_2_VERSION);
    return ctx;
}

// 1. 拆分出的地址解析函数
int resolve_remote_addr(const char *host, int port, struct sockaddr_in *addr) {
    struct hostent *he;
    memset(addr, 0, sizeof(*addr));

    if ((he = gethostbyname(host)) == NULL) {
        herror("gethostbyname");
        return -1;
    }

    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    memcpy(&addr->sin_addr, he->h_addr_list[0], he->h_length);
    return 0;
}

static int create_unconnected_udp_socket(void) {
    int sockfd;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    // 客户端也可以绑定所有 IP，以防万一
    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(0);

    if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

static int create_udp_socket(const char *host, int port) {
    int sockfd;
    struct sockaddr_in addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    /*
    if (inet_pton(AF_INET, host, &addr.sin_addr) <= 0) {
        perror("embedway inet_pton");
        close(sockfd);
        return -1;
    }
    */
    // 替换 inet_pton 部分
    struct hostent *he;
    if ((he = gethostbyname(host)) == NULL) {
        herror("gethostbyname");
        close(sockfd);
        return -1;
    }
    memcpy(&addr.sin_addr, he->h_addr_list[0], he->h_length);


    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <IP Address> <message>\n", argv[0]);
        exit(1);
    }

    const char *host = argv[1];
    const char *message = argv[2];

    struct sockaddr_in remote_addr;
    if (resolve_remote_addr(host, PORT, &remote_addr) != 0) {
        exit(1);
    }
    openssl_init();

    SSL_CTX *ctx = create_dtls_client_ctx();
    if (!ctx) {
        openssl_cleanup();
        exit(1);
    }

    int sock = create_unconnected_udp_socket();
    if (sock < 0) {
        SSL_CTX_free(ctx);
        openssl_cleanup();
        exit(1);
    }

    SSL *ssl = SSL_new(ctx);
    // 告诉 OpenSSL 预期的主机名是 localhost
    SSL_set_tlsext_host_name(ssl, "localhost");
    if (!ssl) {
        ERR_print_errors_fp(stderr);
        close(sock);
        SSL_CTX_free(ctx);
        openssl_cleanup();
        exit(1);
    }
    X509_VERIFY_PARAM *param = SSL_get0_param(ssl);
    X509_VERIFY_PARAM_set1_host(param, "localhost", 0);

    // 2. 解决 MTU 问题
    SSL_set_options(ssl, SSL_OP_NO_QUERY_MTU);
    SSL_set_mtu(ssl, 1200);

    BIO *bio = BIO_new_dgram(sock, BIO_NOCLOSE);
    //BIO_ctrl(bio, BIO_CTRL_DGRAM_SET_CONNECTED, 0, NULL);
    BIO_dgram_set_peer(bio, (struct sockaddr *)&remote_addr);

    SSL_set_bio(ssl, bio, bio);

    int ret = SSL_connect(ssl);
    if (ret <= 0) {
        int err = SSL_get_error(ssl, ret);
        fprintf(stderr, "DTLS handshake failed. Ret: %d, SSL Error: %d\n", ret, err);

        switch (err) {
            case SSL_ERROR_SYSCALL:
                perror("SSL_ERROR_SYSCALL (Check network/firewall/server logs)");
                break;
            case SSL_ERROR_SSL:
                fprintf(stderr, "SSL_ERROR_SSL: Protocol error\n");
                ERR_print_errors_fp(stderr);
                break;
            default:
                ERR_print_errors_fp(stderr);
                break;
        }
        SSL_free(ssl);
        close(sock);
        SSL_CTX_free(ctx);
        openssl_cleanup();
        exit(1);
    }

    int len = strlen(message);
    if (SSL_write(ssl, message, len) != len) {
        fprintf(stderr, "SSL_write failed\n");
        ERR_print_errors_fp(stderr);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(sock);
        SSL_CTX_free(ctx);
        openssl_cleanup();
        exit(1);
    }

    char buf[MAXDATASIZE];
    int numbytes = SSL_read(ssl, buf, sizeof(buf) - 1);
    if (numbytes <= 0) {
        fprintf(stderr, "SSL_read failed\n");
        ERR_print_errors_fp(stderr);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(sock);
        SSL_CTX_free(ctx);
        openssl_cleanup();
        exit(1);
    }

    buf[numbytes] = '\0';
    printf("Server Message: %s\n", buf);

    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sock);
    SSL_CTX_free(ctx);
    openssl_cleanup();
    return 0;
}
