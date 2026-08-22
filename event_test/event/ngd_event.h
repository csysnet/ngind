#ifndef NGD_EVENT
#define NGD_EVENT
//
#include "ngd_core.h"
//
#define NGD_EVENT_MAX 1024
#define NGD_EVENT_WAIT_TIME -1
//
typedef struct ngd_event_t ngd_event_t;

//
int ngd_event_module_init(void);
int ngd_event_module_add(conn_t *c);
int ngd_event_module_del(conn_t *c);
//
int ngd_event_module_enable_write(conn_t *c);
int ngd_event_module_disable_write(conn_t *c);
//
conn_t *ngd_event_get_active_conn(ngd_event_t *ev);
int ngd_event_module_loop(int timeout)
{
    int n = epoll_wait(timeout);
    for (int i=0; i<n; i++)
    {
        conn_t *c = epoll_events[i].data.ptr;
        uint32_t evflag = epoll_events[i].flags;
        if (evflag & EPOLLIN)
            c->on_read = true;
        if (evflag & EPOLLOUT)
            c->on_write = true;
        c->handler(c);
    }
}
//
#endif
//

// for (;;)
// {
//     int timeout = get_next_timeout();
//     ngd_event_module_loop(timeout);
//     timer_handler(timeout);
// }
