#ifndef SSL_H
#define SSL_H

#include <openssl/ssl.h>

#include "core.h"

typedef struct pool_t pool_t;
typedef struct pool

typedef struct ssl_ctx_t {
    SSL_CTX *ctx;
} ssl_ctx_t;

typedef struct ssl_conn_t {
    SSL *ssl;
    ssl_ctx_t *ctx;
}

//ctx
ssl_ctx_t *ssl_ctx_create(pool_t *pool);
int ssl_ctx_cert(ssl_ctx_t *ctx, str_t *cert, str_t *key);
void ssl_ctx_destroy(ssl_ctx_t *ctx);

//per conn
ssl_conn_t *ssl_conn_create(pool_t *pool, ssl_ctx_t *ctx);
int ssl_conn_set_fd(ssl_conn_t *sc, int fd);
int ssl_conn_handshake(ssl_conn_t *sc);
ssize_t ssl_recv(ssl_conn_t *sc, u_char *buf, size_t size);
ssize_t ssl_send(ssl_conn_t *sc, u_char)

#endif
