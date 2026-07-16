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
    //
    r = pool_alloc(c->pool, sizeof(req_t));
    r->header_in = b;
    r->headers = pool_alloc(c->pool, sizeof(map_t));
    map_init(r->headers, c->pool);
    r->smethod = pool_alloc(c->pool, sizeof(str_t));
    r->suri = pool_alloc(c->pool, sizeof(str_t));
    r->sver = pool_alloc(c->pool, sizeof(str_t));
    //
    c->pdata = r;
    //
    rev->handler = http_proc_reqline;
    http_proc_reqline(rev);
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
    //
    for (;;)
    {
        n = http_read_req(rev);
        rc = http_parse_reqline(r, r->header_in);
        if (rc == NGD_OK) {
            r->smethod->p = r->start_method;
            r->smethod->len = r->end_method - r->start_method;
            r->suri->p = r->start_uri;
            r->suri->len = r->end_uri - r->start_uri;
            r->sver->p = r->start_ver;
            r->sver->len = r->end_ver - r->start_ver;
            http_proc_headers(rev);
            rev->handler = http_proc_headers;
            break;
        }

        if (rc == NGD_AGAIN)
            continue;
    }
    //
    return NGD_OK;
}

int
http_proc_headers(event_t *rev)
{

    conn_t *c;
    req_t *r;
    buf_t *b;
    str_t *key, *value;
    ssize_t n;
    int rc;
    //
    c = rev->pdata;
    r = c->pdata;
    b = r->header_in;
    //
    printf("diff: %lu\n", b->last - b->pos);
    printf("reach proc header\n\n");
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
            ps(key);
            //
            value = pool_alloc(c->pool, sizeof(str_t));
            value->p = r->start_value;
            value->len = r->end_value - r->start_value;
            // printf("valuelen: %lu\n", value->len);
            printf(": ");
            ps(value);
            printf("\n");
            //
            map_insert(r->headers, key, value);
            //
            continue;
        }

        if (rc == HTTP_PARSE_HEADER_DONE) {
            key = pool_alloc(c->pool, sizeof(str_t));
            //
            str_from_chars(key, "Host");
            value = map_get(r->headers, key);
            // ps(value);
            //
            str_from_chars(key, "Connection");
            value = map_get(r->headers, key);
            // ps(value);
            //
            printf("\n\nreach done\n\n");
            str_from_chars(key, "Transfer-Encoding");
            value = map_get(r->headers, key);
            // if (value)
            //     ps(value);

        }


        if (rc == NGD_AGAIN)
            continue;

    }
}
int http_proc_body(event_t *rev);
int http_build_req(event_t *wev);
