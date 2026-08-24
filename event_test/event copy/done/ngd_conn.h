#ifndef NGD_CONN_H
#define NGD_CONN_H
//
#include "ngd_core.h"
#include "ngd_event.h"
#include "ngd_timer.h"
//
#define NGD_CONN_LISTEN_BACKLOG 128
#define NGD_CONN_LISTEN_PORT 8080
//
typedef struct ngd_conn_t ngd_conn_t;
//
void ngd_conn_module_init(void (*init_conn)(ngd_conn_t *));
void ngd_conn_module_start(void);
//
int ngd_conn_init(ngd_conn_t *c,
                   int state,
                   void (*handler)(ngd_conn_t *),
                   void *data,
                   uint64_t timeout_ms);
void ngd_conn_close(ngd_conn_t *c);
void ngd_conn_enable_write(ngd_conn_t *c);
void ngd_conn_disable_write(ngd_conn_t *c);
void ngd_conn_reset_timeout(ngd_conn_t *c, uint64_t timer_ms);
//
ssize_t ngd_conn_send(ngd_conn_t *c, u_char *buf, size_t len);
ssize_t ngd_conn_recv(ngd_conn_t *c, u_char *buf, size_t len);
//
int ngd_conn_get_state(ngd_conn_t *c);
void ngd_conn_set_state(ngd_conn_t *c, int state);
void *ngd_conn_get_data(ngd_conn_t *c);
//
bool ngd_conn_is_read(ngd_conn_t *c);
bool ngd_conn_is_write(ngd_conn_t *c);
bool ngd_conn_is_timeout(ngd_conn_t *c);
//
#endif NGD_CONN_H
