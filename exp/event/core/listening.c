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
ngd_open_listening_sockets(ngd_cycle_t *cycle)
{
    int opt;
    ngd_listening_t *ls;

    opt = 1;
    for (int i=0; i<cycle->nlistening; i++)
    {
        ls = cycle->listenings[i];
        ls->fd = socket(AF_INET, SOCK_STREAM, 0);

        setsockopt(ls->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        bind(ls->fd, (struct sockaddr *)&ls->addr, ls->socklen);

        listen(ls->fd, ls->backlog);

    }

    return NGD_OK;
}


void
ngd_close_listening_sockets(ngd_cycle_t *cycle)
{
    for (int i=0; i<cycle->nlistening; i++)
    {
        close(cycle->listenings[i]->fd);
    }
}
