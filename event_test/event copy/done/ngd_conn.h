#ifndef NGD_CONN_H
#define NGD_CONN_H
//
#include "ngd_core.h"
#include "ngd_event.h"
#include "ngd_timer.h"
//
#define NGD_CONN_SET(c, fd, handler, data)
//
typedef struct ngd_conn_t ngd_conn_t;
//
struct ngd_conn_t {
    int fd;
    int state;
    ngd_timer_t timer;
    ngd_event_t event;
    void (*handler)(ngd_conn_t *);
    void *data;
    bool on_read;
    bool on_write;
    bool on_timeout;
};
//
void ngd_conn_module_init(void);
void ngd_conn_module_start(void);
//
ngd_conn_t *ngd_conn_create(int fd);
void ngd_conn_close(ngd_conn_t *c);
void ngd_conn_enable_write(ngd_conn_t *c);
void ngd_conn_disable_write(ngd_conn_t *c);
void ngd_conn_reset_timeout(ngd_conn_t *c, uint64_t timer_ms);
//
#endif NGD_CONN_H
