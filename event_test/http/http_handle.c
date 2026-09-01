int
ngd_http_read_request(ngd_http_t *c)
{
    ngd_http_t *http;
    ngd_buf_t *b;
    int ret;
    void *p;
    ssize_t bytes_recved;
    //
    http = c->data;
    b = http->inbuf;

    //
    if (b->last == b->end) {
        switch (b->end - b->start) {
            case NGD_HTTP_INBUF_SMALL:
                p = pool_alloc(http->pool, NGD_HTTP_INBUF_MEDIUM);
                ngd_str_cpy(b->pos, b->last - b->pos, p, NGD_HTTP_INBUF_MEDIUM);
                ngd_buf_init(b, p, NGD_HTTP_INBUF_MEDIUM);
                break;
            case NGD_HTTP_INBUF_MEDIUM:
                p = pool_alloc(http->poo, NGD_HTTP_INBUF_LARGE);
                ngd_str_cpy(b->pos, b->last - b->pos, p, NGD_HTTP_INBUF_LARGE);
                ngd_buf_init(b, p, NGD_HTTP_INBUF_LARGE);
                break;
            case NGD_HTTP_INBUF_LARGE:
                ngd_str_cpy(b->pos, b->last - b->pos, b->start, NGD_HTTP_INBUF_LARGE);
                b->pos = b->last - b->pos;
                b->last = b->start;
                break;
        }
    }
    ret = ngd_conn_recv(c, b->last, b->end - b->last, &bytes_recved);
    if (ret == NGD_OK) {
        b->last += bytes_recved;
        http->recved_each += bytes_recved;
    }
    return ret;
    //

}


int
ngd_http_handle_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    enum {
        ps_start=NGD_STATE_START,
        ps_reqline,
        ps_headers,
        ps_body,
        ps_build_resp,
        ps_compress_resp,
        ps_send_resp,
    } state;
    int ret;
    ngd_buf_t *b;
    size_t n;
    //
    http = ngd_conn_get_data(c);
    state = http->state;
    b = http->inbuf;
    ret = NGD_AGAIN;
    //
    for (;;)
    {
        //
        if (ret == NGD_AGAIN) {
            if (b->last == b->end) {
                n = b->last - b->pos;
                switch (b->end - b->start) {
                    case NGD_HTTP_INBUF_SMALL:
                        p = pool_alloc(http->pool, NGD_HTTP_INBUF_MEDIUM);
                        ngd_str_cpy(b->pos, n, p, NGD_HTTP_INBUF_MEDIUM);
                        ngd_buf_init(b, p, NGD_HTTP_INBUF_MEDIUM);
                        b->pos += n;
                        b->last = b->pos;
                        break;
                    case NGD_HTTP_INBUF_MEDIUM:
                        p = pool_alloc(http->pool, NGD_HTTP_INBUF_LARGE);
                        ngd_str_cpy(b->pos, b->last - b->pos, p, NGD_HTTP_INBUF_LARGE);
                        ngd_buf_init(b, p, NGD_HTTP_INBUF_LARGE);
                        b->pos += n;
                        b->last = b->pos;
                        break;
                    case NGD_HTTP_INBUF_LARGE:
                        ngd_str_cpy(b->pos, b->last - b->pos, b->start, NGD_HTTP_INBUF_LARGE);
                        b->pos += n;
                        b->last = b->pos;
                        break;
                }
            }
            ret = ngd_conn_recv(c, b->last, b->end - b->last, &n);

            if (ret == NGD_OK) {
                b->last += n;
                rev->recved_each += n;
            } else if (ret == NGD_AGAIN) {
                return;
            } else if (ret == NGD_ERR || ret == NGD_CLOSED) {
                ngd_http_close_conn(http);
            }

        }


        switch (state)
        {
            case ps_start:
                state = ps_reqline;
                break;
            case ps_reqline:
                ret = ngd_http_handle_reqline(http);
                //
                if (ret == NGD_AGAIN) {
                    if (http->recved_each > NGD_HTTP_LIMIT_REQLINE) {
                        ngd_http_close_conn(c);
                        return;
                    }
                    continue;
                }
                //
                if (ret == NGD_OK) {
                    http->smethod.data = http->method_start;
                    http->smethod.len = http->method_end - http->method_start;
                    http->suri.data = http->uri_start;
                    http->suri.len = http->uri_end - http->uri_start;
                    http->sver.data = http->ver_start;
                    http->sver.len = http->ver_end - http->ver_start;
                    http->recved_each += n;
                    http->state_each = 0;
                    state = ps_headers;
                }

                if (ret == NGD_ERR) {
                    ngd_http_close_conn(http);
                }
                break;
            case ps_headers:
                ret = ngd_http_handle_headers(http);
                str = pool_alloc()
                if (ret == NGD_OK) {
                    str->data = http->key_start;
                    str->len = http->key_end - key_start;
                    ret = ngd_list_append(http->headers, str);
                    if (ret == NGD_ERR)
                        ngd_http_close_conn();
                        return;

                }
                if (ret == NGD_HTTP_FULL_HEADER_DONE) {

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

        if (ret == NGD_ERR) {
            ngd_http_close_conn(http);
        }
    }

    //
    return NGD_OK;
// }
