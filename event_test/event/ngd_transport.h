#ifndef NGD_TRANSPORT_H
#define NGD_TRANSPORT_H
//
#include "ngd_core.h"
//
#define NGD_TRANSPORT_EVENT_MAX_GET 1024
//
typedef struct ngd_conn_t ngd_conn_t;
typedef struct ngd_timer_t ngd_timer_t;
//connection
struct ngd_conn_t {
    int fd;
    int state;
    ngd_timer_t timer;
    void (*handler)(ngd_conn_t *);
    void *data;
    bool on_read;
    bool on_write;
    bool on_timeout;
};
void ngd_conn_module_init(void);
ngd_conn_t *ngd_conn_create(int fd);
void ngd_conn_close(ngd_conn_t *c);
//timer
struct ngd_timer_t {
    uint64_t expire;
    ngd_conn_t *conn;
    ngd_timer_t *next;
    bool active;
};
void ngd_timer_module_init(void);
int ngd_timer_regis(ngd_conn_t *c, uint64_t timeout_ms);
int ngd_timer_reset(ngd_conn_t *c, uint64_t timeout_ms);
void ngd_timer_expire(uint64_t now);
int ngd_timer_next_timeout(void);
uint64_t ngd_timer_get_now(void);
//event
int ngd_event_module_init(void);
int ngd_event_regis(ngd_conn_t *c);
int ngd_event_unregis(ngd_conn_t *c);
int ngd_event_enable_write(ngd_conn_t *c);
int ngd_event_disable_write(ngd_conn_t *c);
int ngd_event_loop(int timeout);
//transport
int ngd_transport_module_init(void);
//
#endif
