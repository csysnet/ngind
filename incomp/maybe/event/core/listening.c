#include "core.h"
#include <stdlib.h>

ngd_listening_t *
ngd_create_listening(uint16_t port)
{
    ngd_listening_t *ls;

    ls = malloc(sizeof(ngd_listening_t));
    memset(ls, 0, sizeof(ngd_listening_t));

    ls->fd = -1;
    ls->backlog = 511;

    ls->addr.sin_family = AF_INET;
    ls->addr.sin_addr.s_addr = INADDR_ANY
    ls->addr.sin_port = htons(port);
    ls->socklen = sizeof(struct sockaddr_in);

    return ls;
}
int
listening_open(listening_t *ls)
{
    int opt;

    opt = 1;
    ls->fd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(ls->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bind(ls->fd, (struct sockaddr *)&ls->addr, ls->socklen);

    listen(ls->fd, ls->backlog);

    return NGD_OK;
}


void
listening_close(listening_t *ls)
ngd_close_listening_sockets(ngd_cycle_t *cycle)
{
    close(ls->fd);
}
