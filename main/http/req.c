#include "req.h"
#include "buf.h"
#include "str.h"

int
http_wait_req(event_t *rev)
{
    conn_t *c;
    req_t *r;
    buf_t *b;
    //
    c = rev->pdata;
    //
    b = pool_alloc(c->pool, sizeof(buf_t));
    buf_init(b, MAX_INBUF);
    //
    r = pool_alloc(c->pool, sizeof(req_t));
    r->header_in = b;
    r->smethod = pool_alloc(c->pool, sizeof(str_t));
    r->suri = pool_alloc(c->pool, sizeof(str_t));
    r->sver = = pool_alloc(c->pool, sizeof(str_t));
    //
    c->pdata = r;
    //
    rev->handler = http_proc_reqline;
    http_proc_reqline(rev);
    //
    return NGD_OK
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
http_proc_reqline(event_t *rev)
{

}
int http_proc_headers(event_t *rev);
int http_proc_body(event_t *rev);
int http_build_req(event_t *wev);
