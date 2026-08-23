void
ngd_http_init_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    //
    c->state = NGD_STATE_START;
    c->data = pool_alloc(c->pool, sizeof(ngd_http_t));
    c->handler = ngd_http_handle_conn;
    ngd_timer_module_add(c, 60000)// 60s
    //

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
    state = c->state;
    http = c->data;
    for (;;)
    {
        switch (state)
        {
            case ps_start:
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

int
ngd_http_handle_reqline(ngd_http_t *http)
{
    enum {
        ps_start=NGD_STATE_START,
        ps_method,
        ps_space_uri,
        ps_uri,
        ps_space_ver,
        ps_ver
    } state;
    int ret;
    ssize_t n;
    //
    state = http->state;
    //
    for (;;)
    {
        switch (state)
        {
            case ps_start:
                break;
            case ps_method:
                break;
            case ps_space_uri:
                break;
            case ps_uri:
                break;
            case ps_space_ver:
                break;
            case ps_ver:
                break;
        }
    }
}
