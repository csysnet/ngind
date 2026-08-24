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
    state = c->state;
    http = c->data;
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
    ngd_buf_t *b;
    u_char *p;
    int ret;
    ssize_t n;
    //
    state = http->state;
    b = http->inbuf;
    //
    for (p = b->pos; p < b->last; p++)
    {
        switch (state)
        {
            case ps_start:
                http->method_start = p;
                break;
            case ps_method:
                if (*p == ' ') {
                    http->method_end = p;
                    state = ps_space_uri;
                }
                break;
            case ps_space_uri:
                http->uri_start = p;
                state = ps_uri;
                break;
            case ps_uri:
                if (*p == ' ') {
                    http->uri_end = p;
                    state = ps_space_ver;
                }
                break;
            case ps_space_ver:
                http->ver_start = p;
                state = ps_ver;
                break;
            case ps_ver:
                if (c == '\n') {
                    http->ver_end = p - 1;
                    goto done;
                }
                break;
        }
    }
    b->pos = b->last;
    http->state = state;
    return NGD_AGAIN;
done:
    b->pos = p + 1;
    http->state = ps_start;
    return NGD_OK;
}
//
int
ngd_http_handle_headers(ngd_http_t *http)
{
    enum {
        ps_start=NGD_STATE_START,
        ps_key,
        ps_space_value,
        ps_value
    } state;
    ngd_buf_t *b;
    u_char *p;
    int ret;
    ssize_t n;
    //
    state = http->state;
    b = http->inbuf;
    //
    for (;;)
    {
        switch (state)
        {
            case ps_start:
                if (*p == '\r')
                    break;
                if (*p == '\n')
                    goto header_done;
                http->key_start = p;
                state = ps_key;
                break;
            case ps_key:
                if (*p == ' ') {
                    http->key_end = p - 1;
                    state = ps_space_value;
                }
                break;
            case ps_space_value:
                http->value_start = p;
                state = ps_value;
                break;
            case ps_value:
                if (*p == '\n') {
                    http->value_end = p - 1;
                    goto done;
                }
                break;
        }
    }
    //
    b->pos = b->last;
    http->state = state;
    return NGD_AGAIN;
done:
    b->pos = p + 1;
    http->state = ps_start;
    return NGD_OK;
header_done:
    b->pos = p + 1;
    http->state = ps_start;
    return NGD_HTTP_FULL_HEADER_DONE;
}
int
ngd_http_handle_body(ngd_http_t *http)
{
    enum {
        ps_start=NGD_STATE_START,
        ps_zero,
        ps_zero_cr,
        ps_zero_cr_lf,
        ps_zero_cr_lf_cr,
    } state;
    ngd_buf_t *b;
    u_char *p;
    int ret;
    ssize_t n;
    //
    state = http->state;
    b = http->inbuf;
    //
    for (p = b->pos; p < b->last; p++)
    {
        switch (state)
        {
            case ps_start:
                if (*p == '0')
                    state = ps_
        }
    }
}
int ngd_http_build_resp(ngd_http_t *http);
int ngd_http_compress_resp(ngd_http_t *http);
int ngd_http_send_resp(ngd_http_t *http);
