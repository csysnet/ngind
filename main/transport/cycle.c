#include "cycle.h"

#include "pool.h"
#include "event.h"
#include "listen.h"
#include "ssl.h"

cycle_t *
cycle_create(int nlisten)
{
    pool_t *pool;
    cycle_t *cycle;

    pool = pool_create();

    cycle = pool_alloc(pool, sizeof(cycle_t));

    cycle->pool = pool;
    cycle->listens = pool_alloc(pool, sizeof(listen_t) * nlisten);
    cycle->nlisten = nlisten;
    cycle->ilisten = 0;

    return cycle;
}

int
cycle_add_listen(cycle_t *cycle, uint16_t port, int ssl, int (*handler)(conn_t *c))
{
    listen_t *ls;

    ls = pool_alloc(cycle->pool, sizeof(listen_t));
    ls->handler = handler;
    ls->port = port;
    if (!ssl)
        ls->ssl_ctx = NULL;
    else {
        ls->ssl_ctx = pool_alloc(cycle->pool, sizeof(ssl_ctx_t));
        ssl_ctx_init(ls->ssl_ctx);
        if (ls->ssl_ctx == NULL) {
            return NGD_ERR;
        }
    }

    if (cycle->ilisten >= cycle->nlisten)
        return NGD_ERR;

    cycle->listens[cycle->ilisten] = ls;
    cycle->ilisten++;

    return NGD_OK;
}

int
cycle_open_listens(cycle_t *cycle)
{
    int n;

    n = cycle->ilisten;
    for (int i=0; i<n; i++)
        if (listen_init((listen_t *)cycle->listens[i]) == NGD_ERR)
            return NGD_ERR;
    return NGD_OK;
}


int
cycle_register_listens(cycle_t *cycle)
{
    int n;

    n = cycle->ilisten;
    for (int i=0; i<n; i++)
        if (event_regis_listen((listen_t *)cycle->listens[i]) == NGD_ERR)
            return NGD_ERR;

    return NGD_OK;
}

int
cycle_init_event(void)
{
    event_init();
    return NGD_OK;
}
int
cycle_event_loop(void)
{
    event_loop();
    return NGD_OK;
}

int
cycle_close(cycle_t *cycle)
{
    int n;

    n = cycle->ilisten + 1;
    for (int i=0; i<n; i++)
        if (listen_close((listen_t *)cycle->listens[i]) == NGD_ERR)
            return NGD_ERR;

    pool_destroy(cycle->pool);

    return NGD_OK;
}
