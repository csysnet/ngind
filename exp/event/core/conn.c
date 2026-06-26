#include <stdlib.h> // malloc
#include <string.h> // memset
#include <unistd.h> // close
#include <fcntl.h> //fcntl
#include <sys/socket.h> // socket, bind, listen, accept
#include <netinet/in.h> // sockaddr_in, htons
#include <stdio.h>
#include "core.h"


int
ngd_conn_set_nonblocking(int fd)
{
    int flags;
    flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}


ngd_conn_t *
ngd_conn_create(int fd)
{
    ngd_conn_t *c;

    c = malloc(sizeof(ngd_conn_t));
    c->fd = fd;

    c->read = malloc(sizeof(ngd_event_t));
    c->read->active = 1;
    c->write = malloc(sizeof(ngd_event_t));
    c->write->active = 1;

    c->send = ngd_unix_send;
    c->recv = ngd_unix_recv;

    return c;
}

ngd_conn_t *
ngd_conn_listener_create(int port, int backlog, int(*handler)(ngd_conn_t *c))
{
    struct sockaddr_in addr;
    int lfd;
    ngd_listener_t *ls;
    ngd_conn_t *lc;

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(lfd, backlog);
    printf("Listening on port %d...\n", port);

    ls = malloc(sizeof(ngd_listener_t));
    ls->fd = lfd;
    ls->handler = handler;

    lc = ngd_conn_create(lfd);
    lc->read->handler = ngd_event_accept;
    lc->read->pdata = lc;
    lc->pdata = ls;
    return lc;
}

int
ngd_conn_accept(int lfd)
{
    int cli_fd;
    cli_fd = accept(lfd, (struct sockaddr *)NULL, NULL);
    return cli_fd;
}


int
ngd_conn_close(int fd)
{
    close(fd);
}
