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
#define NGD_CONN_IS_READ (c) ((c)->on_read)
#define NGD_CONN_IS_WRITE (c) ((c)->on_write)
#define NGD_CONN_IS_TIMEOUT (c) ((c)->on_timeout)
#define NGD_CONN_GET_STATE (c) ((c)->state)
#define NGD_CONN_SET_STATE (c, state) ((c)->state = state)
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
ssize_t ngd_conn_send(ngd_conn_t *c, u_char *buf, size_t len);
ssize_t ngd_conn_recv(ngd_conn_t *c, u_char *buf, size_t len);
//
#endif NGD_CONN_H
