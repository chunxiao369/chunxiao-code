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
#include <openssl/hmac.h>
#include <openssl/rand.h>

#define PORT           1234
#define MAXDATASIZE    1024

void openssl_init(void) {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void openssl_cleanup(void) {
    EVP_cleanup();
}

// 用于生成 Cookie 的简单密钥
unsigned char cookie_secret[16];
int cookie_initialized = 0;

int generate_cookie(SSL *ssl, unsigned char *cookie, unsigned int *cookie_len) {
    unsigned char *buffer, result[EVP_MAX_MD_SIZE];
    unsigned int length = 0, resultlength;
    union {
        struct sockaddr_storage ss;
        struct sockaddr_in6 s6;
        struct sockaddr_in s4;
    } peer;

    /* 初始化随机密钥 (仅一次) */
    if (!cookie_initialized) {
        if (!RAND_bytes(cookie_secret, 16)) {
            fprintf(stderr, "Error generating cookie secret\n");
            return 0;
        }
        cookie_initialized = 1;
    }

    /* 获取对端地址 */
    (void) BIO_dgram_get_peer(SSL_get_rbio(ssl), &peer);

    /* 计算数据长度 */
    length = 0;
    switch (peer.ss.ss_family) {
        case AF_INET:
            length += sizeof(struct in_addr);
            length += sizeof(in_port_t);
            break;
        case AF_INET6:
            length += sizeof(struct in6_addr);
            length += sizeof(in_port_t);
            break;
        default:
            length += sizeof(peer.ss);
            break;
    }

    buffer = (unsigned char*) OPENSSL_malloc(length);
    if (buffer == NULL) return 0;

    /* 填充地址信息到 buffer */
    switch (peer.ss.ss_family) {
        case AF_INET:
            memcpy(buffer, &peer.s4.sin_port, sizeof(in_port_t));
            memcpy(buffer + sizeof(in_port_t), &peer.s4.sin_addr, sizeof(struct in_addr));
            break;
        case AF_INET6:
            memcpy(buffer, &peer.s6.sin6_port, sizeof(in_port_t));
            memcpy(buffer + sizeof(in_port_t), &peer.s6.sin6_addr, sizeof(struct in6_addr));
            break;
        default:
            memcpy(buffer, &peer.ss, length);
            break;
    }

    /* 使用 HMAC 计算 Cookie */
    HMAC(EVP_sha1(), (const void*) cookie_secret, 16,
         (const unsigned char*) buffer, length, result, &resultlength);
    OPENSSL_free(buffer);

    memcpy(cookie, result, resultlength);
    *cookie_len = resultlength;

    return 1;
}

int verify_cookie(SSL *ssl, const unsigned char *cookie, unsigned int cookie_len) {
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned int resultlength;

    // 重新生成 cookie 并对比
    if (generate_cookie(ssl, result, &resultlength) &&
        cookie_len == resultlength &&
        memcmp(result, cookie, resultlength) == 0) {
        return 1;
    }
    return 0;
}

SSL_CTX *create_dtls_server_ctx(void) {
    const SSL_METHOD *method = DTLS_server_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        return NULL;
    }
    // --- 新增: 注册 Cookie 回调 ---
    SSL_CTX_set_cookie_generate_cb(ctx, generate_cookie);
    SSL_CTX_set_cookie_verify_cb(ctx, verify_cookie);

    if (SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM) <= 0 ||
        SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM) <= 0 ||
        !SSL_CTX_check_private_key(ctx)) {
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return NULL;
    }

    SSL_CTX_set_min_proto_version(ctx, DTLS1_2_VERSION);
    return ctx;
}

static int create_udp_socket(int port) {
    int sockfd;
    struct sockaddr_in addr;
    int opt = 1;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

static SSL *dtls_server_accept(SSL_CTX *ctx, int udp_fd,
                               struct sockaddr_storage *client_addr,
                               socklen_t *client_len) {
    char buf[1500];
    *client_len = sizeof(struct sockaddr_storage);
    ssize_t n = recvfrom(udp_fd, buf, sizeof(buf), MSG_PEEK,
                         (struct sockaddr *)client_addr, client_len);
    if (n <= 0) {
        perror("recvfrom");
        return NULL;
    }

    BIO *bio = BIO_new_dgram(udp_fd, BIO_NOCLOSE);
    if (!bio) return NULL;

    //BIO_ctrl(bio, BIO_CTRL_DGRAM_SET_CONNECTED, 0, client_addr);
    BIO_dgram_set_peer(bio, client_addr);

    SSL *ssl = SSL_new(ctx);
    if (!ssl) {
        BIO_free(bio);
        return NULL;
    }

    SSL_set_bio(ssl, bio, bio);

    if (SSL_accept(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        return NULL;
    }

    return ssl;
}

int main(void) {
    openssl_init();

    SSL_CTX *ctx = create_dtls_server_ctx();
    if (!ctx) {
        openssl_cleanup();
        return 1;
    }

    int udp_fd = create_udp_socket(PORT);
    if (udp_fd < 0) {
        SSL_CTX_free(ctx);
        openssl_cleanup();
        return 1;
    }

    printf("Listening on UDP port %d...\n", PORT);

    while (1) {
        struct sockaddr_storage client_addr;
        socklen_t client_len;

        SSL *ssl = dtls_server_accept(ctx, udp_fd, &client_addr, &client_len);
        if (!ssl) {
            fprintf(stderr, "DTLS handshake failed\n");
            continue;
        }

        char msg[MAXDATASIZE];
        int num = SSL_read(ssl, msg, sizeof(msg) - 1);
        if (num <= 0) {
            ERR_print_errors_fp(stderr);
            SSL_shutdown(ssl);
            SSL_free(ssl);
            continue;
        }

        msg[num] = '\0';
        printf("You got a message \"%s\" from %s\n", msg, inet_ntoa(((struct sockaddr_in *)&client_addr)->sin_addr));

        const char *reply = "Welcome to my server.\n";
        if (SSL_write(ssl, reply, strlen(reply)) <= 0) {
            ERR_print_errors_fp(stderr);
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);

        if (!strcmp(msg, "quit")) {
            break;
        }
    }

    close(udp_fd);
    SSL_CTX_free(ctx);
    openssl_cleanup();
    return 0;
}
