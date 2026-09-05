#include "ngd_http.h"
//
void
ngd_http_init_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    ngd_pool_t *pool;
    //
    pool = ngd_pool_create();
    if (pool == NULL) {
        ngd_conn_close(c);
        return;
    }
    //
    http = ngd_pool_alloc(pool, sizeof(*http));
    if (http == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }
    //
    http->pool = pool;
    http->state = NGD_STATE_START;
    http->state_req = NGD_STATE_START;
    http->fd_temp = -1;
    //header
    ngd_buf_init(
        &http->headbuf,
        ngd_pool_alloc(pool, NGD_HTTP_HEADER_BUF_LEN),
        NGD_HTTP_HEADBUF_SIZE
    );
    if (http->inbuf->start == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }
    //body
    ngd_buf_init(
        &http->bodybuf,
        ngd_pool_alloc(pool, NGD_HTTP_BODY_BUF_LEN),
        NGD_HTTP_BODY_LEN
    );
    if (http->bodybuf.start == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }
    //
    ngd_buf_init(
        &http->outbuf,
        ngd_pool_alloc(pool, NGD_HTTP_OUTBUF_LEN),
        NGD_HTTP_OUTBUF_LEN
    );
    if (http->outbuf.start == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }
    //
    ngd_conn_init(
        c,
        ngd_http_handle_conn,
        http,
        NGD_HTTP_TIMEOUT_INIT
    );
}
//
void
ngd_http_handle_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    ngd_buf_t *b;
    int ret;
    size_t n;
    enum {
        ps_start = NGD_STATE_START,
        ps_reqline,
        ps_headers,
        ps_body,
        ps_build_resp,
        ps_send_resp,
        ps_close
    } state;
    //
    http = ngd_conn_get_data(c);
    b = http->inbuf;
    state = http->state;
    //
    for (;;)
    {
        switch (state)
        {
            case ps_start:
                state = ps_reqline;
                break;
            case ps_reqline:
                ret = ngd_http_parse_reqline(http);
                if (ret == NGD_OK)
                    state = ps_headers;

                if (ret == NGD_ERR)
                    state = ps_build_resp;

                if (ret == NGD_AGAIN) {
                    if (b->last == b->end && b->pos == b->start)
                    {

                    }
                }
                break;
        }
    }
    http->state = state;
    //
}
