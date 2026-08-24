#include "ngd_http.h"
//
int
ngd_http_init_conn(ngd_conn_t *c)
{
    ngd_pool_t *pool;
    ngd_http_t *http;
    //
    pool = ngd_pool_create();
    //
    http = pool_alloc(pool, sizeof(ngd_http_t));
    http->pool = pool;
    http->state = NGD_STATE_START;
    //
    ngd_conn_init(c,
                  NGD_STATE_START,
                  ngd_http_handle_conn,
                  (void *)http,
                  60000);//60s
    //
    return NGD_OK;
}
//
int
ngd_http_handle_conn(ngd_conn_t *c)
{
    enum {
        ps_start=NGD_STATE_START,
        ps_reqline,
        ps_headers,
        ps_body,
        ps_build_resp,
        ps_compress_resp,
        ps_send_resp,
    } state;
    ngd_http_t *http;
    int ret;
    ssize_t n;
    //
    state = ngd_conn_get_state(c);
    http = ngd_conn_get_data(c);
    //
    for (;;)
    {
        switch (state)
        {
            case ps_start:
                http->state = NGD_STATE_START;
                state = ps_reqline;

                break;
            case ps_reqline:
                ret = ngd_http_handle_reqline(http);
                if (ret == NGD_OK) {
                    http->smethod.data = http->method_start;
                    http->smethod.len = http->method_end - http->method_start;
                    http->suri.data = http->uri_start;
                    http->suri.len = http->uri_end - http->uri_start;
                    http->sver.data = http->ver_start;
                    http->sver.len = http->ver_end - http->ver_start;
                    state = ps_headers;
                }
                if (ret == NGD_AGAIN) {

                }

                if (ret == NGD_ERR) {

                }
                break;
            case ps_headers:
                ret = ngd_http_handle_headers(http);
                if (ret == NGD_OK) {
                }
                if (ret == NGD_AGAIN) {

                }
                if (ret == NGD_ERR) {

                }
                break;
            case ps_body:
                ret = ngd_http_handle_body(http);
                break;
            case ps_build_resp:
                ret = ngd_http_build_resp(http);
                break;
            case ps_compress_resp:
                ret = ngd_http_compress_resp(http);
                break;
            case ps_send_resp:
                ret = ngd_http_send_resp(http);
                break;
        }
    }

    //
    return NGD_OK;
}
//
int ngd_http_close_conn(ngd_conn_t *c);
