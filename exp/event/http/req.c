#include "http.h"

#include "stdlib.h"

int
ngd_http_init_conn(ngd_conn_t *c)
{
    size_t n;
    ngd_req_t *r = malloc(sizeof(ngd_req_t));
    ngd_buf_t *b = malloc(sizeof(ngd_buf_t));

    n = 100000;
    b->start = malloc(n);
    b->pos = b->start;
    b->last = b->start;
    b->end = b->start + n;
    r->header_in = b;

    c->read->handler = ngd_http_proc_reqline;
    c->read->pdata = c;
    c->write->handler = ngd_http_empty_handler;
    c->write->pdata = c;


    c->pdata = r;

    c->closed = 1;

    return NGD_OK;
}

int
ngd_wait_req(ngd_event_t *rev)
{
    rev->handler = ngd_http_proc_reqline;
    ngd_http_proc_reqline(rev);
    return NGD_OK;
}

int
ngd_http_empty_handler(ngd_event_t *wev)
{
    return NGD_OK;
}
int
ngd_http_block_reading(ngd_event_t *rev)
{
    return NGD_OK;
}

int
ngd_http_proc_reqline(ngd_event_t *rev)
{
    int rc;
    ngd_conn_t *c;
    ngd_req_t *r;
    ssize_t n;

    c = rev->pdata;
    r = c->pdata;

    for (;;) {
        n = r->header_in->last - r->header_in->pos;
        if (n > 0)
            return n;

        n = c->recv(c, r->header_in->last, r->header_in->end - r->header_in->last);

        r->header_in->last += n;

        rc = ngd_http_parse_reqline(r, r->header_in);

        if (rc == NGD_OK)
            ngd_http_proc_headers(rev);

        if (rc == NGD_AGAIN)
            continue;
    }

    return NGD_OK;
}

int
ngd_http_proc_headers(ngd_event_t *rev)
{
    return NGD_OK;
}
