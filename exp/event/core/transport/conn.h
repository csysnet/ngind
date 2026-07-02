#ifndef CONN_T
#define CONN_T

#include "core.h"

typedef struct event_t event_t;
typedef struct listening_t listening_t;
typedef struct ssl_conn_t ssl_conn_t;
typedef struct pool_t pool_t;
typedef struct cycle_t cycle_t;

typedef struct conn_t {
    int fd;
    listening_t *listening;

    pool_t *pool;

    even_t *read;
    event_t *write;

    ssize_t (*recv)(conn_t *c, u_char *buf, size_t size);
    ssize_t (*send)(conn_t *c, u_char *buf, size_t size);

    ssl_conn_t *ssl;

    void *pdata;
} conn_t;

int conn_create(conn_t *c, int fd, listening_t *ls);
int conn_close(conn_t *c);

int conn_accept(int lfd);
int conn_set_nonblocking();

#endif
