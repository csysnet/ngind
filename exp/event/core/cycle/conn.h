#ifndef CONN_T
#define CONN_T

#include "core.h"

typedef struct listen_t listen_t;
typedef struct pool_t pool_t;
typedef struct event_t event_t;

typedef struct conn_t {
    int fd;
    listen_t *listen;

    pool_t *pool;

    event_t *read;
    event_t *write;

    ssize_t (*send)(conn_t *c, u_char *buf, size_t size);
    ssize_t (*recv)(conn_t *c, u_char *buf, size_t size);
} conn_t;
//
int conn_create(int fd, listen_t *ls);
int conn_close(conn_t *c);

int conn_accept(int lfd);
int conn_set_nonblock(int fd);
#endif
