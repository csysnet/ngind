#ifndef CYCLE_H
#define CYCLE_H

#include "core.h"

typedef struct pool_t pool_t;
typedef struct listen_t listen_t;
typedef struct conn_t conn_t;

typedef struct cycle_t {
    pool_t *pool;
    listen_t **listens;
    size_t nlisten;
    size_t ilisten;
} cycle_t;

cycle_t *cycle_create(int nlisten);
int cycle_add_listen(cycle_t *cycle, uint16_t port, int ssl, int (*handler)(conn_t *c));
int cycle_open_listens(cycle_t *cycle);
int cycle_init_event(void);
int cycle_register_listens(cycle_t *cycle);
int cycle_event_loop(void);
int cycle_close(cycle_t *cycle);

#endif
