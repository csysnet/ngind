#include "http.h"

#include "stdlib.h"
#include "stdio.h"

static void
ps(void *buf, size_t buflen)
{
    for (int i = 0; i<buflen; i++)
        printf("%c", ((char*)buf)[i]);

}

int
ngd_http_read_req(ngd_event_t *rev)
{
    ngd_conn_t *c;
    ngd_req_t *r;
    ssize_t n;
    c = rev->pdata;
    r = c->pdata;
    n = r->header_in->last - r->header_in->pos;

    if (n > 0)
        return n;

    n = c->recv(c, r->header_in ->last, r->header_in->end - r->header_in->last);
    r->header_in->last += n;
    return n;
}


int
ngd_http_init_conn(ngd_conn_t *c)
{
    c->send = ngd_unix_send;
    c->recv = ngd_unix_recv;

    // printf("reach ngd_http_init_conn\n");
    c->read->handler = ngd_wait_req;
    c->read->pdata = c;
    c->write->handler = ngd_http_empty_handler;
    c->write->pdata = c;

    return NGD_OK;
}

int
ngd_wait_req(ngd_event_t *rev)
{
    // printf("reach ngd_wait_req\n");
    //init request
    size_t n;
    ngd_buf_t *b;
    ngd_req_t *r;
    ngd_conn_t *c;

    n = 100000;

    b = malloc(sizeof(ngd_buf_t));
    b->start = malloc(n);
    b->pos = b->start;
    b->last = b->start;
    b->end = b->start + n;

    r = malloc(sizeof(ngd_req_t));
    r->header_in = b;
    r->smethod = malloc(sizeof(ngd_str_t));
    r->suri = malloc(sizeof(ngd_str_t));
    r->sver = malloc(sizeof(ngd_str_t));

    c = rev->pdata;
    c->pdata = r;

    rev->handler = ngd_http_proc_reqline;
    ngd_http_proc_reqline(rev);

    return NGD_OK;
}


int
ngd_http_proc_reqline(ngd_event_t *rev)
{
    // printf("reach ngd_http_proc_reqline\n");

    int rc;
    ngd_conn_t *c;
    ngd_req_t *r;
    ssize_t n;

    c = rev->pdata;
    r = c->pdata;

    for (;;) {
        n = ngd_http_read_req(rev);

        rc = ngd_http_parse_reqline(r, r->header_in);
        if (rc == NGD_OK){
            r->smethod->pdata = r->start_method;
            r->smethod->len = r->end_method - r->start_method;
            r->suri->pdata = r->start_uri;
            r->suri->len = r->end_uri - r->start_uri;
            r->sver->pdata = r->start_ver;
            r->sver->len = r->end_ver - r->start_ver;
            ps(r->smethod->pdata, r->smethod->len);printf(" ");ps(r->suri->pdata, r->suri->len);printf(" ");ps(r->sver->pdata, r->sver->len);printf("\n");

            ngd_http_proc_headers(rev);
            rev->handler = ngd_http_proc_headers;
            break;
        }

        if (rc == NGD_AGAIN)
            continue;
    }

    return NGD_OK;
}

int
ngd_http_proc_headers(ngd_event_t *rev)
{
    ngd_conn_t *c;
    ngd_req_t *r;
    ssize_t n;
    int rc;

    c = rev->pdata;
    r = c->pdata;

    for (;;)
    {
        n = ngd_http_read_req(rev);
        if (n == NGD_AGAIN)
            break;

        rc = ngd_http_parse_header_line(r, r->header_in);
        if (rc == NGD_OK) {
            ps(r->start_key, r->end_key - r->start_key);
            printf(":");
            ps(r->start_value, r->end_value-r->start_value);
            printf("\n");
            continue;
        }

        if (rc == NGD_AGAIN) {
            printf("HEADER FULL DONE");
            continue;
        }

        if (rc == NGD_PARSE_HEADER_DONE) {
            printf("HEADER FULL DONE");
        }
    }
}

int
ngd_http_empty_handler(ngd_event_t *wev)
{
    // printf("reach ngd_http_empty_handler\n");
    return NGD_OK;
}
int
ngd_http_block_reading(ngd_event_t *rev)
{
    return NGD_OK;
}
