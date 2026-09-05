#include <sys/epoll.h> //epoll_event, epoll_create1(), epoll_ctl(), epoll_wait()
//
#include "event.h"
#include "listen.h"
#include "conn.h"
#include "pool.h"
#include "ssl.h"
//
static int epfd;
static struct epoll_event events[MAX_EVENTS];

static int
event_set(conn_t *c, int op, int evflag)
{
    struct epoll_event ee;
    //
    ee.events = evflag;
    ee.data.ptr = c;
    epoll_ctl(epfd, op, c->fd, &ee);
    //
    return NGD_OK;
}

int
event_init(void)
{
    epfd = epoll_create1(0);
    return NGD_OK;
}


int
event_regis_listen(listen_t *ls)
{
    conn_t *lc;
    //
    lc = conn_create(ls->fd, ls);
    conn_set_nonblock(ls->fd);
    lc->read->pdata = lc;
    lc->read->handler = event_accept;
    event_set(lc, EPOLL_CTL_ADD, EPOLLIN);
    //
    return NGD_OK;
}

int
event_accept(event_t *rev)
{

    conn_t *lc, *c;
    listen_t *ls;
    int cli_fd;
    //
    lc = rev->pdata;
    ls = lc->listen;
    //
    cli_fd = conn_accept(ls->fd);
    conn_set_nonblock(cli_fd);
    c = conn_create(cli_fd, ls);
    if (!ls->ssl_ctx)
        ls->handler(c);
    else {
        c->ssl = pool_alloc(c->pool, sizeof(ssl_conn_t));
        ssl_conn_init(c->ssl, ls->ssl_ctx);
        ssl_conn_setfd(c->ssl, cli_fd);
        // c->read->handler = ssl_handshake_handler
    }
    event_set(c, EPOLL_CTL_ADD, EPOLLIN);
    //
    return NGD_OK;
}


int
event_enable_write(event_t *wev)
{
    conn_t *c;

    c = wev->pdata;

    event_set(c, EPOLL_CTL_MOD, EPOLLIN | EPOLLOUT);
    return NGD_OK;
}

int
event_disable_write(event_t *wev)
{
    conn_t *c;
    //
    c = wev->pdata;
    event_set(c, EPOLL_CTL_MOD, EPOLLIN);
    return NGD_OK;
}
int
event_del(event_t *ev)
{
    conn_t *c;
    //
    c =  ev->pdata;
    event_set(c, EPOLL_CTL_DEL, 0);
    return NGD_OK;
}


int
event_loop(void)
{
    int n;
    conn_t *c;
    uint32_t evflag;
    event_t *rev, *wev;
    //
    for (;;)
    {
        printf("reach event looop\n");
        n = epoll_wait(epfd, events, MAX_EVENTS, EVENT_WAITTIME);
        for (int i=0; i<n; i++)
        {
            c = events[i].data.ptr;
            evflag = events[i].events;
            //
            rev = c->read;
            wev = c->write;

            //
            if (evflag & EPOLLIN)
                rev->handler(rev);
            if (evflag & EPOLLOUT)
                wev->handler(wev);

        }
    }
}
