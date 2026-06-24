#ifndef NGD_CORE_H
#define NGD_CORE_H

#include "buf.h"
#include "conn.h"
#include "ev.h"
#include "listening.h"
#include "cycle.h"
//dir
#define NGD_OK 0
#define NGD_AGAIN -1

typedef struct ngd_str_t {
    u_char  *pdata;
    size_t len;
} ngd_str_t;


//io
#include <sys/socket.h>
static ssize_t
ngd_unix_recv(ngd_conn_t *c, u_char *buf, size_t size)
{
    return recv(c->fd, buf, size, 0);
}

static ssize_t
ngd_unix_send(ngd_conn_t *c, u_char *buf, size_t size)
{
    return send(c->fd, buf, size, 0);
}
#endif
