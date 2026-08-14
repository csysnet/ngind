#ifndef NGD_CONN_H
#define NGD_CONN_H
//
#include "ngd_core.h"
//
typedef struct ngd_conn_t ngd_conn_t;
//
struct ngd_conn_t {
    int fd;
    //
    void (*handler)(ngd_conn_t *);
    void *data;
    //states
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
