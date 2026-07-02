#ifndef NGD_CYCLE_H
#define NGD_CYCLE_H

typedef struct pool_t pool_t;

typedef struct {

    pool_t *pool;
    ngd_listening_t **listenings;
    int nlistening;
    int (*handler)(ngd_conn_t *c)
} ngd_cycle_t;

int ngd_init_cycle(cycle);


#endif
