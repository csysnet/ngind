#ifndef CYCLE_H
#define CYCLE_H

#include "core.h"

typedef struct pool_t pool_t;
typedef struct listening_t listening_t;
typedef struct event_ctx_t event_ctx_t;

typedef struct {
    pool_t *pool;
    listening_t *listenings;
    size_t listen_cap;
    size_t nlistening;

    event_ctx_t *event_ctx_t

};

int cycle_init();
int cycle_add_listening(port, ssl, handler);
int cycle_open_listenings();
void cycle_close_listenings();
int cycle_register_listening();


cycle_init()
cycle_add_listening(8080, true, http_init_connection);
{
    listening_t *ls;
    ls = pool_alloc(cycle->pool, sizeof(listening_t));
    http =
    if (true)

}
cycle_add_listeing(404, false, http_init_connetion);
cycle_add_liste

extern cycle_t cycle;

#endif
