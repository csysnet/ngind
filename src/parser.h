#ifndef PARSER_H
#define PARSER_H
#include "./structs.h"
//GET FUL HEA
//AFTER
//quakhu+condi=fut
//> 2: 1state

void cli_init(cli_t *cli)
{
    cli->sfd..
    cli->state = STATE_METHOD;
    cli->byte_recv = 0;


}

//
void handle_method(cli_t *cli);
void handle_url(cli_t *cli);
void handle_ver(cli_t *cli);
void handle_ver_cr(cli_t *cli);
void handle_key(cli_t *cli);
void handle_value(cli *cli);
void handle_value_cr(cli_t *cli);
void handle_value_cr_lf(cli_t *cli);
void handle_cr_lf_cr(cli_t *cli);


void handle_body(cli_t *cli);

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

void
handle_cli(cli *cli, void *tempbuf, size_t n_read)
{
 copy temp to cli
 then update st or call some what
}

while (1)
{
    ssize_t n_read = read(fd, tempbuf, len);

    if (n_read > 0) handle_cli(cli, tempbuf, n_read);
    else if (n_read == 0) { close(fd); break;}
    else {
        if (errno = EAGAIN || errno = EWOULDBLOCK) break;
        else { close(fd); break; }
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
