#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
//
#include "conn.h"
#include "pool.h"
#include "event.h"

static ssize_t
unix_send(conn_t *c, u_char *buf, size_t size)
{
    return send(c->fd, (void *)buf, size, 0);
}

static ssize_t
unix_recv(conn_t *c, u_char *buf, size_t size)
{
    return recv(c->fd, (void *)buf, size, 0);
}

conn_t *
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
    c->send = unix_send;
    c->recv = unix_recv;
    // c->ssl = NULL;
    // C->pdata = NULL;
    //
    return c;
}

int
conn_close(conn_t *c)
{
    close(c->fd);
    pool_destroy(c->pool);
    return NGD_OK;
}

int
conn_accept(int lfd)
{
    int cli_fd;
    cli_fd = accept(lfd, NULL, NULL);
    return cli_fd;
}
int
conn_set_nonblock(int fd)
{
    int flags;
    flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}
