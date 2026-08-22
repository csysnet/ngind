#include "ngd_transport.h"
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
static
//
void ngd_conn_module_init(void);
//
ngd_conn_t *
ngd_conn_create(int fd)
{
    ngd_conn_t *c;
    //
    c = ngd_conn_get();
    c->fd = fd;
    //
    return c;
}
void
ngd_conn_close(ngd_conn_t *c)
{
    close(c->fd);
    ngd_conn_release(c);
}
