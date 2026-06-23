#include "ngd_req.h"
#include "ngd_pseudo.h"



int
ngd_wait_req(ngd_event_t *rev)
{

}


int
ngd_read_req(ngd_event_t *rev)
{
    ngd_conn_t *c;
    ngd_req_t *r;
    ssize_t n;

    c = rev->pdata;
    r = c->pdata;

    n = r->header_in->last - r->header_in->pos;

    if (n > 0) {
        return n;
    }

    n = c->recv(c, r->header_in->last, r->header_in->end - r->header_in->last);

    r->header_in->last += n;
}


int
ngd_proc_reqline(ngd_event_t *rev)
{
    int rc;
    ngd_conn_t *c;
    ngd_req_t *r;
    ssize_t n;

    c = rev->pdata;
    r = c->pdata;

    for (;;) {
        n = ngd_read_req(rev);

        if (n == NGD_AGAIN) {
            break;
        }

        rc = ngd_parse_reqline(r, r->header_in);

        if  (rc == NGD_OK) {
            ngd_proc_headers(rev);
            rev = ngd_proc_headers;
            return
        }

        if (rc == NGD_AGAIN) {
            continue;
        }

    }

}


int
ngd_proc_headers(ngd_event_t *rev)
{
    ngd_conn_t *c;
    ngd_req_t *r;
    ssize_t n;
    int rc;

    c = rev->pdata;
    r = c->pdata;

    for (;;) {

        n = ngd_read_req(rev);

        if (n == NGD_AGAIN) {
            break;
        }

        rc = ngd_parse_header_line(r, r->header_in);

        if (rc == NGD_OK) {
            r->smethod.p = r->start_method;
            r->smethod.len = r->end_method - r->start_method;
            r->suri.p = r->start_uri;
            r->suri.len = r->end_uri - r->start_uri;
            r->sver.p = r->start_ver;
            r->sver.len = r->end_ver - r->start_ver;
            continue;
        }

        if (rc == NGD_AGAIN) {
            continue;
        }

        if (rc == NGD_PARSE_HEADER_DONE) {

        }


    }
}
