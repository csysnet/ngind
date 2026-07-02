//sys include
#include <stdio.h>

#include <stdlib.h> //maloc
#include <stdint.h> //uint32_t
#include <sys/epoll.h> //epoll_create1, epoll_ctl, epoll_wait
//usr include
#include "core.h"

//code
static int epfd;
static struct epoll_event *events;
static int nevents;

int
ngd_event_init(void)
{

    cycle->event_ctx_t->epfd =

    nevents = 1024;
    epfd = epoll_create1(0);
    events = malloc(sizeof(struct epoll_event) * nevents);
}

int
ngd_event_loop(void)
{

    ngd_conn_t *c;
    ngd_event_t *rev;
    ngd_event_t *wev;
    uint32_t revents;
    int n;
    while (1)
    {
        n = epoll_wait(epfd, events, 128, -1);
        for (int i=0;i<n;i++)
        {
            c = events[i].data.ptr;

            revents = events[i].events;
            rev = c->read;
            wev = c->write;

            if (revents & EPOLLIN)
                rev->handler(rev);

            if (revents & EPOLLOUT)
                wev->handler(wev);
        }
    }
    return NGD_OK;
}

int
ngd_event_accept(ngd_event_t *rev)
{
    int cli_fd;
    ngd_conn_t *c, *lc;
    ngd_listener_t *ls;

    lc = rev->pdata;

    ls = lc->pdata;
    cli_fd = ngd_conn_accept(ls->fd);
    ngd_conn_set_nonblocking(cli_fd);
    c = ngd_conn_create(cli_fd);
    ls->handler(c);

    ngd_event_register_conn(c);
    return NGD_OK;
}

int
ngd_event_register_conn(ngd_conn_t *c)
{
    struct epoll_event ee;

    ee.events = EPOLLIN;
    ee.data.ptr = c;
    epoll_ctl(epfd, EPOLL_CTL_ADD, c->fd, &ee);
    return NGD_OK;
}

int
ngd_event_enable_write(ngd_event_t *wev)
{
    ngd_conn_t *c;
    struct epoll_event ee;

    c = wev->pdata;

    ee.events = EPOLLIN | EPOLLOUT;
    ee.data.ptr = c;

    epoll_ctl(epfd, EPOLL_CTL_MOD, c->fd, &ee);
}
int
ngd_event_disable_write(ngd_event_t *wev)
{
    ngd_conn_t *c;
    struct epoll_event ee;

    c = wev->pdata;

    ee.events = EPOLLIN;
    ee.data.ptr = c;

    epoll_ctl(epfd, EPOLL_CTL_MOD, c->fd, &ee);
}
x
int
ngd_event_del(ngd_event_t *ev)
{
    ngd_conn_t *c;

    c = ev->pdata;

    epoll_ctl(epfd, EPOLL_CTL_DEL, c->fd, NULL);
}


//ngd_event_init_listening_sockets
int
event_register_listening_conn(conn_t *lc)
{

    c->read->handler = event_accept;

}



int
ngd_event_proc_init(ngd_cycle_t *cycle)
{
    ngd_listening_t *ls;
    ngd_conn_t *c;
    for (int i=0; i<cycle->nlistening; i++)
    {
        ls = cycle->listening[i];
        c = ngd_get_connection(ls->fd);
        c->listening = ls;

        c->read->handler = ngd_event_accept;
        ngd_event_add(c);
        ngd_event_register_conn(lc);
    }

    return NGD_OK
}
