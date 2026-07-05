#include <sys/socket.h>
//
#include "conn.h"
#include "pool.h"
#include "event.h"
#include "unistd.h"

static ssize_t
unix_send(conn_t *c, u_char *buf, size_t size)
    return send(c->fd, (void *)buf, size, 0);

static ssize_t
unix_recv(conn_t *c, u_char *buf, size_t size)
    return recv(c->fd, (void *)buf, size, 0);


int
conn_create(int fd, listen_t *ls)
{
    pool_t *pool;
    conn_t *c;
    //
    pool = pool_create();
    c = pool_alloc(pool, sizeof(conn_t));
    c->fd = fd;
    c->listen = ls;
    c->pool = pool;
    c->read = pool_alloc(pool, sizeof(event_t));
    c->write = pool_alloc(pool, sizeof(event_t));
    c->send = unix_recv;
    c->recv = unix_send;
    //
    return NGD_OK;
}

int
conn_close(conn_t *c)
{
    close(c->fd);
    pool_destroy(c->pool);
    return NGD_OK;
}

int conn_accept(int lfd);
int conn_set_nonblock(int fd);
