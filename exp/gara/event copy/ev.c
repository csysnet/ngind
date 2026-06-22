#include "ev.h"
#include "conn.h"

static int epfd;
static struct epoll_event *events;
static int nevents;

int
ngd_event_init()
{
    nevents = 1024;
    epfd = epoll_create1(0);
    events = malloc(sizeof(struct epoll_event) * nevents);
}

int
ngd_event_proc()
{
    ngd_conn_t *c;
    ngd_event_t *rev;
    ngd_event_t *wev;
    uint32_t revents;
    int i, n;

    n = epoll_wait(epfd, events, 128, -1);
    for (i=0;i<n;i++)
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

int
ngd_event_loop()
{
    while (1)
    {
        ngd_event_proc();
    }

}

int
ngd_event_accept(ngd_event_t *rev)
{
    ngd_conn_t *lc, *c;
    int cli_fd;

    lc = rev->pdata;
    cli_fd = ngd_conn_accept(lc->fd);

    c = malloc(sizeof(ngd_conn_t));
    c->fd = ngd_conn_accept(lc->fd);

    rev->handler(c);
    ngd_event_add_conn(c);
}

int
ngd_event_add_conn(ngd_conn_t *c)
{
    struct epoll_event ee;

    ee.events = EPOLLIN;
    ee.data.ptr = c;

    epoll_ctl(epfd, EPOLL_CTR_ADD, c->fd, &ee);

}

int
ngd_event_enable_write(ngd_event_t *ev)
{
    struct epoll_event ee;

    ee.events = EPOLLIN | EPOLLOUT;
    ee.data.ptr = ev;

    epoll_ctl(epfd, EPOLL_CTL_MOD, ev->fd, &ee);
}
int
ngd_event_disable_write(ngd_event_t *ev)
{
    struct epoll_event ee;

    ee.events = EPOLLIN;
    ee.data.ptr = ev;

    epoll_ctl(epfd, EPOLL_CTL_MOD, ev->fd, &ee);
}

int
ngd_event_del(ngd_event_t *ev)
{
    epoll_ctl(epfd, EPOLL_CTL_DEL, ev->fd, NULL);
}
