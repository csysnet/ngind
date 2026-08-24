//
#include <sys/socket.h>   // sockaddr, socket(), setsockopt(), bind(), listen()
#include <netinet/in.h>   // sockaddr_in, htons(), htonl(), INADDR_ANY
#include <unistd.h>       // close()
//
#include "listen.h"
#include "str.h"


int
listen_init(listen_t *ls)
{
    struct sockaddr_in addr;
    int opt;

    //
    ls->fd = socket(AF_INET, SOCK_STREAM, 0);
    //
    opt = 1;
    setsockopt(ls->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    //
    str_zeros((void *)&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(ls->port);
    bind(ls->fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    //
    listen(ls->fd, BACKLOG);
    //
    return NGD_OK;
}

int
listen_close(listen_t *ls)
{
    close(ls->fd);
    return NGD_OK;
}
