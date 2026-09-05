#ifndef LISTENING_T
#define LISTENING_T

#include <core.h>

typedef struct ssl_ctx_t ssl_ctx_t;
typedef struct conn_t conn_t;

typedef struct listening_t {
    int fd;
    ssl_ctx_t *ssl_ctx;
    int (*handler)(conn_t *c);
    unsigned ssl:1;
} listening_t;


int listening_init(listening_t *ls, uint16_t port);
int listening_open(listening_t *ls);
void listening_close(listeing_t *ls);

#endif
