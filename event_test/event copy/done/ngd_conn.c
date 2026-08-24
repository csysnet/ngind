#include "ngd_conn.h"
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
//
void
ngd_conn_module_init(void)
{
    ngd_event_module_init();
    ngd_timer_module_init();
}
void
ngd_conn_module_start(void)
{

}
//
ngd_conn_t *
ngd_conn_create(int fd)
{
    ngd_conn_t *c;
    //
    c = ngd_conn_get();
    c->fd = fd;

    ngd_event_regis(&c->event, c->fd, c->handler, c->data);
    ngd_timer_regis(&c->event, c->handler, c->data);
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
