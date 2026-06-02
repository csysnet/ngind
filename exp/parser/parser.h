#ifndef PARSER_H
#define PARSER_H
#include "./structs.h"
//GET FUL HEA
//AFTER
//quakhu+condi=fut
//> 2: 1state
// it should know it own linear byte that other control
// which portion you want, you have

void cli_init(cli_t *cli)
{
    cli->sfd..
    cli->state = STATE_METHOD;
    cli->byte_recv = 0;


}

void
conn_init(conn_t *c)
{
    c->fd
}



void handle_state(cli_t *cli, );

void
shit(conn_t conn, )
{
    ssize_t n_read;
    STATE state = STATE_BEGIN;
    total
    for (..)
    {
        n_read = read(fd, buf, buflen);

    }
}

void conn_handle()



void
http_handle(conn_t con, )

//when readable, as much as we can






void
handle_cli(cli *cli)
{
    ssize_t n_read;

    while (1)
    {
        n_read = read(cli->sfd, (sfd->buf) + (sfd->w), )

        if (n_read > 0) handle_cli(cli, tempbuf, n_read);
        else if (n_read == 0) { close(fd); break;}
        else {
            if (errno = EAGAIN || errno = EWOULDBLOCK) break;
            else { close(fd); break; }
        }
    }
}

ssize_t
recv_msg(int fd, void *buf, size_t buflen)
{
    ssize_t n_read;

    n_read = read(fd, (char *)buf + total, buflen - total);

    handle_state(cli *cli);
    if (cli->state == STATE_DONE);

    total += n_read;
    return total;
}

void
append_req()



#endif
