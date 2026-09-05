#ifndef SSL_H
#define SSL_H

#include <openssl/ssl.h>
//
#include "core.h"

typedef struct str_t str_t;

typedef struct ssl_ctx_t {
    SSL_CTX *ctx;
} ssl_ctx_t;

typedef struct ssl_conn_t {
    SSL *ssl;
} ssl_conn_t;
//
int ssl_ctx_init(ssl_ctx_t *ctx);
int ssl_ctx_cert(ssl_ctx_t *ctx, str_t *cert, str_t *key);
//
int ssl_conn_init(ssl_conn_t *sc, ssl_ctx_t *ctx);
int ssl_conn_setfd(ssl_conn_t *sc, int fd);
int ssl_conn_handshake(ssl_conn_t *sc);

ssize_t ssl_recv(ssl_conn_t *sc, u_char *buf, size_t size);
ssize_t ssl_send(ssl_conn_t *sc, u_char *buf, size_t size);

#endif
