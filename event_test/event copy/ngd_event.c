#include <sys/epoll.h>
#include "ngd_transport.h"
//
static int epfd;
static struct epoll_event events[NGD_TRANSPORT_EVENT_MAX_GET];
//
static int
ngd_event_set(ngd_conn_t *c, int op, int evflags)
{
    struct epoll_event ee;
    //
    ee.events = evflag;
    ee.data.ptr = c;
    epoll_ctl(epfd, op, c->fd, &ee);
    //
    return NGD_OK;
}
//
int
ngd_event_module_init(void)
{
    epfd = epoll_create1(0);
    return NGD_OK;
}
int
ngd_event_regis(ngd_conn_t *c)
{
    ngd_event_set(c, EPOLL_CTL_ADD, EPOLLIN);
    return NGD_OK;
}
int
ngd_event_unregis(ngd_conn_t *c)
{
    ngd_event_set(c, EPOLL_CTL_DEL, 0);
    return NGD_OK;
}
int
ngd_event_enable_write(ngd_conn_t *c)
{
    ngd_event_set(c, EPOLL_CTL_ADD, EPOLLIN | EPOLLOUT);
    return NGD_OK;
}
int
ngd_event_disable_write(ngd_conn_t *c)
{
    ngd_event_set(c, EPOLL_CTL_ADD, EPOLLIN);
    return NGD_OK;
}
int
ngd_event_loop(int timeout)
{
    int n;
    //
    n = epoll_wait(epfd, events, sizeof(events), timeout);
    //
    for (int i=0; i<n; i++)
    {
        evflags = events[i].events;
        c = events[i].data.ptr;
        //
        if (evflags & EPOLLIN)
            c->on_read = true;
        if (evflags & EPOLLOUT)
            c->on_write = true;
        c->handler(c);
    }
    //
    return NGD_OK;
}
