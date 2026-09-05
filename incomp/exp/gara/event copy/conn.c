#include "./conn.h"

ngd_conn_t *
ngd_conn_create(int fd)
{
    ngd_conn_t *c;

    c = malloc(sizeof(ngd_conn_t));
    c->fd = fd;
    return c;
}

ngd_conn_t *
ngd_conn_listener_create(int port, int backlog, int(*handler)(ngd_conn_t *c))
{
    struct sockaddr_in addr;
    int lfd;
    ngd_listener_t *ls;
    ngd_conn_t *c;

    ls = malloc(sizeof(ngd_listener_t));
    lfd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    memset(&addr, 0, sizeof(struct sockaddr_in));
    bind(lfd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
    listen(lfd, backlog);
    ls->fd = lfd;
    ls->handler = handler;

    c = malloc(sizeof(ngd_conn_t));
    c->pdata = ls;
    return c;
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
