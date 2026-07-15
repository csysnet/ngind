// //
#include "req.h"
#include "buf.h"
#include "str.h"
#include "map.h"
#include "event.h"
#include "conn.h"
#include "pool.h"
//
int
http_wait_req(event_t *rev)
{
    printf("reach wait\n");
    conn_t *c;
    req_t *r;
    buf_t *b;
    ssize_t n;
    //
    c = rev->pdata;
    //
    b = pool_alloc(c->pool, sizeof(buf_t) + MAX_INBUF);
    b->start = b + sizeof(buf_t);
    b->pos = b->start;
    b->last = b->start;
    b->end = b->start + MAX_INBUF;

    printf("start: %lu\n", b->start);
    printf("pos: %lu\n", b->pos);
    printf("last: %lu\n", b->last);
    printf("end: %lu\n", b->end);
    printf("diff: %lu\n", b->end - b->last);
    printf("call revc         dsadsadsadwqdwqdwqdwqdqwdqwdwq");
    n = c->recv(c, b->start, b->end - b->last);
    b->last += n;
    printf("n read: %ld\n", n);
    setvbuf(stdout, NULL, _IONBF, 0);
    // for (int i = 0; i<n; i++)
    // *(b->start) = (char)"b";
    // printf("%c\n", (char)*(b->start));
    // printf("escap chacrpr\n");
    //
    r = pool_alloc(c->pool, sizeof(req_t));
    r->header_in = b;
    r->smethod = pool_alloc(c->pool, sizeof(str_t));
    r->suri = pool_alloc(c->pool, sizeof(str_t));
    r->sver = pool_alloc(c->pool, sizeof(str_t));
    //
    c->pdata = r;
    //
    rev->handler = http_proc_reqline;
    // http_proc_reqline(rev);
    //
    return NGD_OK;
}
int
http_empty_handler(event_t *wev)
{
    return NGD_OK;
}

int
http_block_reading(event_t *rev)
{
    return NGD_OK;
}

int
http_read_req(event_t *rev)
{
    conn_t *c;
    req_t *r;
    buf_t *b;
    ssize_t n;
    //
    c = rev->pdata;
    r = c->pdata;
    b = r->header_in;
    //
    n = b->last - b->pos;

    if (n > 0)
        return n;
    n = c->recv(c, b->last, b->end - b->last);
    // printf("n read: %ld\n", n);
    b->last += n;
    return n;
}



int
http_proc_reqline(event_t *rev)
{
    int rc;
    conn_t *c;
    req_t *r;
    ssize_t n;
    buf_t *b;
    //
    c = rev->pdata;
    r = c->pdata;
    b = r->header_in;
    printf("reach proc reqline\n");
    printf("start: %lu\n", b->start);
    printf("pos: %lu\n", b->pos);
    printf("last: %lu\n", b->last);
    printf("end: %lu\n", b->end);
    printf("diff: %lu\n", b->last - b->pos);
    b->start[0] = 'd';
    b->start[1] = 'a';
    b->start[2] = 't';
    b->start[3] = '\0';
    printf("fadsfsda:%s\n", b->start);
    sleep(10000);
    //
    // for (;;)
    // {
    //     n = http_read_req(rev);
        rc = http_parse_reqline(r, r->header_in);
        if (rc == NGD_OK) {
            printf("reach ok");
                sleep(10000000);
            r->smethod->p = r->start_method;
            r->smethod->len = r->end_method - r->start_method;
            r->suri->p = r->start_uri;
            r->suri->len = r->end_uri - r->start_uri;
            r->sver->p = r->start_ver;
            r->sver->len = r->end_ver - r->start_ver;

            http_proc_headers(rev);
            rev->handler = http_proc_headers;
            // break;
        }

        // if (rc == NGD_AGAIN)
        //     continue;
    // }
    //

    return NGD_OK;
}

int
http_proc_headers(event_t *rev)
{
    conn_t *c;
    req_t *r;
    str_t *key, *value;
    ssize_t n;
    int rc;
    //
    c = rev->pdata;
    r = c->pdata;
    //
    for (;;)
    {
        n = http_read_req(rev);
        if (n == NGD_AGAIN)
            break;

        rc = http_parse_header_line(r, r->header_in);
        if (rc == NGD_OK) {
            key = pool_alloc(c->pool, sizeof(str_t));
            key->p = r->start_key;
            key->len = r->end_key - r->start_key;
            //
            value = pool_alloc(c->pool, sizeof(str_t));
            value->p = r->start_value;
            value->len = r->end_value - r->start_value;
            //
            map_insert(r->headers, key, value);
            //
            continue;
        }

        if (rc == HTTP_PARSE_HEADER_DONE) {
            printf("HEADER FULLY DONE\n");
        }


        if (rc == NGD_AGAIN)
            continue;

    }
}
int http_proc_body(event_t *rev);
int http_build_req(event_t *wev);
