#ifndef NGD_CONN_H
#define NGD_CONN_H
//
#include "ngd_core.h"
//
typedef struct ngd_conn_t ngd_conn_t;
typedef struct ngd_timer_t timer;
//
struct ngd_conn_t {
    int fd;
    //outside
    ngd_timer_t timer;
    int state;
    ngd_pool_t *pool;
    void (*handler)(ngd_conn_t *);
    void *data;
    bool on_read;
    bool on_write;
    bool on_timeout;
    //

};
//
void ngd_conn_module_init(void);
conn_t *ngd_conn_create(int fd);
conn_t *ngd_conn_close(conn_t *c);
//
#endif
