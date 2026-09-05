#ifndef LISTEN_H
#define LISTEN_H

#include "core.h"

#define BACKLOG 100

typedef struct ssl_ctx_t ssl_ctx_t;
typedef struct conn_t conn_t;

typedef struct listen_t {
    int fd;

    int (*handler)(conn_t *c);
    ssl_ctx_t *ssl_ctx;
    uint16_t port;
} listen_t;

int listen_init(listen_t *ls);
int listen_close(listen_t *ls);

#endif
