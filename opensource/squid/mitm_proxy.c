// mitm_proxy.c
// Compile with: gcc mitm_proxy.c -o mitm_proxy -levent -levent_openssl -lssl -lcrypto
// Run with: ./mitm_proxy -k ca.key -c ca.pem https 0.0.0.0 8443
// Assumes ca.pem and ca.key exist (CA cert/key trusted by clients).
// iptables -t nat -A PREROUTING -i br0 -p tcp --dport 443 -j REDIRECT --to-port 8443

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/bufferevent_ssl.h>
#include <event2/listener.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/x509v3.h>
#include <openssl/bn.h>
#include <time.h>

struct proxy_conn {
    struct bufferevent *client;
    struct bufferevent *server;
    struct sockaddr_in orig_dst;
};

struct event_base *base;
SSL_CTX *server_ctx;
SSL_CTX *client_ctx;
X509 *ca_cert;
EVP_PKEY *ca_key;

static void init_openssl() {
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
}

static int load_ca(const char *cert_file, const char *key_file) {
    FILE *fp;

    fp = fopen(cert_file, "r");
    if (!fp) {
        perror("fopen cert");
        return -1;
    }
    ca_cert = PEM_read_X509(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!ca_cert) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    fp = fopen(key_file, "r");
    if (!fp) {
        perror("fopen key");
        return -1;
    }
    ca_key = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!ca_key) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    return 0;
}

static void server_read_cb(struct bufferevent *bev, void *arg) {
    struct proxy_conn *conn = (struct proxy_conn *)arg;
    int PRINT_SIZE = 256;
    if (conn->client) {
        struct evbuffer *input = bufferevent_get_input(bev);
        size_t len = evbuffer_get_length(input);
        if (len > 0) {
            char buf[PRINT_SIZE + 1];  // 多一个字节用于null终止，如果打印字符串
            size_t to_copy = (len < PRINT_SIZE) ? len : PRINT_SIZE;
            evbuffer_copyout(input, buf, to_copy);
            buf[to_copy] = '\0';  // 如果作为字符串打印，确保终止
            printf("Decrypted server response (first %zu bytes): %s\n", to_copy, buf);
            //for (size_t i = 0; i < to_copy; i++)
            //    printf("%02x ", (unsigned char)buf[i]);
            // printf("\n");
        }
        bufferevent_write_buffer(conn->client, input);
    }
}

static void event_cb(struct bufferevent *bev, short events, void *arg) {
    struct proxy_conn *conn = (struct proxy_conn *)arg;

    if (events & BEV_EVENT_ERROR) {
        unsigned long err;
        while ((err = bufferevent_get_openssl_error(bev))) {
            char buf[256];
            ERR_error_string_n(err, buf, sizeof(buf));
            fprintf(stderr, "SSL error: %s\n", buf);
        }
        perror("Connection error");
    }

    if (events & (BEV_EVENT_ERROR | BEV_EVENT_EOF)) {
        if (bev == conn->client) {
            if (conn->server) bufferevent_free(conn->server);
            bufferevent_free(conn->client);
        } else {
            if (conn->client) bufferevent_free(conn->client);
            bufferevent_free(conn->server);
        }
        free(conn);
    } else if (events & BEV_EVENT_CONNECTED) {
        // Connection established (for logging if needed)
    }
}

static int sni_cb(SSL *ssl, int *al, void *arg) {
    X509 *server_cert = NULL;
    const char *hostname = SSL_get_servername(ssl, TLSEXT_NAMETYPE_host_name);
    if (!hostname || *hostname == '\0') {
        return SSL_TLSEXT_ERR_NOACK;
    }

    struct proxy_conn *conn = (struct proxy_conn *)SSL_get_ex_data(ssl, 0);

    // Generate server key
    EVP_PKEY *server_key = NULL;
    RSA *rsa = NULL;
    BIGNUM *bn = BN_new();
    if (!bn) goto err;
    BN_set_word(bn, RSA_F4);
    rsa = RSA_new();
    if (!rsa) goto err;
    if (!RSA_generate_key_ex(rsa, 2048, bn, NULL)) goto err;
    server_key = EVP_PKEY_new();
    if (!server_key) goto err;
    if (!EVP_PKEY_assign_RSA(server_key, rsa)) goto err;
    rsa = NULL;

    // Generate cert
    server_cert = X509_new();
    if (!server_cert) goto err;
    X509_set_version(server_cert, 2);
    ASN1_INTEGER_set(X509_get_serialNumber(server_cert), (long)time(NULL));
    X509_gmtime_adj(X509_get_notBefore(server_cert), -86400); // 1 day before
    X509_gmtime_adj(X509_get_notAfter(server_cert), 31536000L); // 1 year

    X509_NAME *subj = X509_NAME_new();
    if (!subj) goto err;
    X509_NAME_add_entry_by_txt(subj, "CN", MBSTRING_ASC, (const unsigned char *)hostname, -1, -1, 0);
    X509_set_subject_name(server_cert, subj);
    X509_NAME_free(subj);

    X509_set_issuer_name(server_cert, X509_get_subject_name(ca_cert));

    if (!X509_set_pubkey(server_cert, server_key)) goto err;

    // Add SAN
    char san_buf[256];
    snprintf(san_buf, sizeof(san_buf), "DNS:%s", hostname);
    X509_EXTENSION *ext = X509V3_EXT_conf_nid(NULL, NULL, NID_subject_alt_name, san_buf);
    if (!ext) goto err;
    X509_add_ext(server_cert, ext, -1);
    X509_EXTENSION_free(ext);

    // Sign
    if (!X509_sign(server_cert, ca_key, EVP_sha256())) goto err;

    // Set to SSL
    if (SSL_use_certificate(ssl, server_cert) <= 0) goto err;
    if (SSL_use_PrivateKey(ssl, server_key) <= 0) goto err;

    // Create server side connection
    SSL *server_ssl = SSL_new(client_ctx);
    if (!server_ssl) goto err;
    if (SSL_set_tlsext_host_name(server_ssl, hostname) != 1) goto err;

    conn->server = bufferevent_openssl_socket_new(base, -1, server_ssl,
                                                  BUFFEREVENT_SSL_CONNECTING,
                                                  BEV_OPT_CLOSE_ON_FREE);
    if (!conn->server) goto err;

    bufferevent_setcb(conn->server, server_read_cb, NULL, event_cb, conn);
    bufferevent_enable(conn->server, EV_READ | EV_WRITE);

    if (bufferevent_socket_connect(conn->server, (struct sockaddr *)&conn->orig_dst, sizeof(conn->orig_dst)) < 0) {
        goto err;
    }

    BN_free(bn);
    return SSL_TLSEXT_ERR_OK;

err:
    if (bn) BN_free(bn);
    if (rsa) RSA_free(rsa);
    if (server_key) EVP_PKEY_free(server_key);
    if (server_cert) X509_free(server_cert);
    if (conn->server) {
        bufferevent_free(conn->server);
        conn->server = NULL;
    }
    ERR_print_errors_fp(stderr);
    return SSL_TLSEXT_ERR_ALERT_FATAL;
}

static void client_read_cb(struct bufferevent *bev, void *arg) {
    struct proxy_conn *conn = (struct proxy_conn *)arg;
    if (conn->server) {
        struct evbuffer *input = bufferevent_get_input(bev);
        bufferevent_write_buffer(conn->server, input);
    }
}


static void accept_cb(struct evconnlistener *listener, evutil_socket_t fd,
                      struct sockaddr *addr, int socklen, void *arg) {
    struct proxy_conn *conn = calloc(1, sizeof(struct proxy_conn));
    if (!conn) {
        close(fd);
        return;
    }

    socklen_t len = sizeof(conn->orig_dst);
    if (getsockopt(fd, SOL_IP, SO_ORIGINAL_DST, (struct sockaddr *)&conn->orig_dst, &len) != 0) {
        perror("getsockopt SO_ORIGINAL_DST");
        close(fd);
        free(conn);
        return;
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &conn->orig_dst.sin_addr, ip, sizeof(ip));
    printf("Original destination: %s:%d\n", ip, ntohs(conn->orig_dst.sin_port));

    SSL *client_ssl = SSL_new(server_ctx);
    if (!client_ssl) {
        close(fd);
        free(conn);
        return;
    }

    SSL_set_ex_data(client_ssl, 0, conn);

    conn->client = bufferevent_openssl_socket_new(base, fd, client_ssl,
                                                  BUFFEREVENT_SSL_ACCEPTING,
                                                  BEV_OPT_CLOSE_ON_FREE);
    if (!conn->client) {
        SSL_free(client_ssl);
        close(fd);
        free(conn);
        return;
    }

    bufferevent_setcb(conn->client, client_read_cb, NULL, event_cb, conn);
    bufferevent_enable(conn->client, EV_READ | EV_WRITE);
}

int main(int argc, char **argv) {
    if (argc != 8 || strcmp(argv[1], "-k") != 0 || strcmp(argv[3], "-c") != 0 || strcmp(argv[5], "https") != 0) {
        fprintf(stderr, "Usage: %s -k <ca.key> -c <ca.pem> https <bind_addr> <port>\n", argv[0]);
        return 1;
    }

    const char *ca_key_file = argv[2];
    const char *ca_cert_file = argv[4];
    const char *bind_addr = argv[6];
    int port = atoi(argv[7]);

    srand(time(NULL));

    init_openssl();

    if (load_ca(ca_cert_file, ca_key_file) != 0) {
        return 1;
    }

    server_ctx = SSL_CTX_new(TLS_server_method());
    if (!server_ctx) {
        ERR_print_errors_fp(stderr);
        return 1;
    }
    SSL_CTX_set_tlsext_servername_callback(server_ctx, sni_cb);
    SSL_CTX_set_security_level(server_ctx, 1);

    client_ctx = SSL_CTX_new(TLS_client_method());
    if (!client_ctx) {
        ERR_print_errors_fp(stderr);
        return 1;
    }
    SSL_CTX_set_verify(client_ctx, SSL_VERIFY_NONE, NULL);

    base = event_base_new();
    if (!base) {
        fprintf(stderr, "Couldn't create event_base\n");
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    inet_pton(AF_INET, bind_addr, &sin.sin_addr);

    struct evconnlistener *listener = evconnlistener_new_bind(base, accept_cb, NULL,
                                                              LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
                                                              -1, (struct sockaddr*)&sin, sizeof(sin));
    if (!listener) {
        fprintf(stderr, "Couldn't create listener\n");
        return 1;
    }

    event_base_dispatch(base);

    evconnlistener_free(listener);
    X509_free(ca_cert);
    EVP_PKEY_free(ca_key);
    SSL_CTX_free(server_ctx);
    SSL_CTX_free(client_ctx);
    event_base_free(base);

    return 0;
}
