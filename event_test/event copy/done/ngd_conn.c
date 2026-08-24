//
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
//
#include "ngd_conn.h"
//
static struct {
    int fd;
    ngd_event_t event;
    void (*init_conn)(ngd_conn_t *);
} listener;
static ngd_conn_t *
listener_accept()
{
    ngd_conn_t *c;
    int flags;
    //
    c = ngd_conn_get();
    c->fd = accept(listener->fd, NULL, NULL);
    //
    flags = fcntrl(c->fd, F_GETFL, 0);
    fcntl(c->fd, F_SETFL, flags | O_NONBLOCK);
    //
    return c;
}
static int
listener_handle_event(ngd_event_t *ev)
{
    uint8_t retflags;
    ngd_conn_t *c;
    //
    retflags = NGD_EVENT_GET_RETFLAGS(ev);
    //
    if (NGD_EVENT_IS(ev, NGD_EVENT_READ)) {
        c = listener_accept();
        listener->init_conn(c)
    }
    //
    return NGD_OK;
}
static void
listener_init(void (*init_conn)(ngd_conn_t *))
{
    struct sockaddr_in addr;
    int opt;
    //
    listener->fd = socket(AF_INET, SOCK_STREAM, 0);
    //
    opt = 1;
    setsockopt(listener->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    //
    str_zeros((void *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(NGD_CONN_LISTEN_PORT);
    bind(listener->fd,
        (struct sockaddr *)&addr,
        sizeof(addr));
    //
    listen(listener->fd, NGD_CONN_LISTEN_BACKLOG);
    //
    ngd_event_regis(listener->event,
                    listener->fd,
                    listener_handle_event,
                    &listener);
}
//
static ngd_conn_t *
ngd_conn_get()
{
    return malloc(sizeof(ngd_conn_t));
}
static int
ngd_conn_release(ngd_conn_t *c)
{
    free(c);
    return NGD_OK;
}
static int
ngd_conn_handle_event(ngd_event_t *ev)
{
    uint8_t retflags;
    ngd_conn_t *c;
    //
    retflags = NGD_EVENT_GET_RETFLAGS(ev);
    c = NGD_EVENT_GET_DATA(ev);
    //
    if (NGD_EVENT_IS(ev, NGD_EVENT_READ)) c->on_read = true;
    if (NGD_EVENT_IS(ev, NGD_EVENT_WRITE)) c->on_write = true;
    //
    c->handler(c);
    //
    return NGD_OK;
}
static int
ngd_conn_handle_timeout(ngd_timer_t *tmr)
{
    ngd_conn_t *c;
    //
    c = NGD_TIMER_GET_DATA(tmr);
    c->timeout = true;
    //
    c->handler(c);
    //
    return NGD_OK;
}
//
void ngd_conn_module_init(void (*init_conn)(ngd_conn_t *))
{
    ngd_event_module_init();
    ngd_timer_module_init();
    //
    listener_init(init_conn);
}
void
ngd_conn_module_start(void)
{
    int timeout;
    uint64_t now;
    void *data;
    //
    for (;;)
    {
        timeout = ngd_timer_module_get_next_timeout();
        ngd_event_module_loop(timeout);
        now = ngd_timer_module_get_now();
        ngd_timer_module_expire(now);
    }
}
//
int
ngd_conn_init(ngd_conn_t *c,
              int state,
              void (*handler)(ngd_conn_t *),
              void *data,
              uint64_t timeout_ms)
{
    c->state = state;
    c->handler = handler;
    c->data = data;
    //
    ngd_event_regis(&c->event, ngd_conn_handle_event, c);
    ngd_timer_regis(&c->timer, ngd_conn_handle_timeout, c, timeout_ms);
    //
    return NGD_OK;
}
ngd_conn_t *
ngd_conn_init(int fd)
{
    ngd_conn_t *c;
    //
    c = ngd_conn_get();
    c->fd = fd;
    //
    ngd_event_regis(&c->event, fd, ngd_conn_handle_event, c);
    ngd_timer_regis(&c->event, ngd_conn_handle_timeout, c);
    //
    return c;
}
void
ngd_conn_close(ngd_conn_t *c)
{
    close(c->fd);
    ngd_timer_unregis(&c->timer);
    ngd_event_unregis(&c->event);
    ngd_conn_release(c);
}
void
ngd_conn_enable_write(ngd_conn_t *c)
{
    ngd_event_enable_write(&c->event);
}
void
ngd_conn_disable_write(ngd_conn_t *c)
{
    ngd_event_disable_write(&c->event);
}
void
ngd_conn_reset_timeout(ngd_conn_t *c, uint64_t timer_ms)
{
    ngd_timer_reset(&c->timer, timer_ms);
}
//
