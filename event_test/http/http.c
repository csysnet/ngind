#include "ngd_http.h"
//
void
ngd_http_init_conn(ngd_conn_t *c)
{
    ngd_pool_t *pool;
    ngd_http_t *http;
    void *p;
    //
    pool = ngd_pool_create();
    if (pool == NULL) {
        ngd_conn_close(c);
        return;
    }
    //
    http = ngd_pool_alloc(pool, sizeof (*http));
    if (http == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }
    //
    http->conn = c;
    http->pool = pool;
    http->state = NGD_STATE_START;
    http->state_parse = NGD_STATE_START;
    http->fd_temp = -1;
    //
    ngd_buf_init(&http->inbuf,
                 p,
                 NGD_HTTP_INBUF_SIZE);
    ngd_conn_init(c,
                  ngd_http_handle_conn,
                  http,
                  60000);
}
//
void
ngd_http_handle_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    ngd_buf_t *b;
    ngd_http_header_t *header;
    ngd_list_node_t *node;
    //
    int ret;
    void *p;
    //
    http = ngd_conn_get_data(c);
    b = http->inbuf;
    //
    if (ngd_conn_on_timeout(c)) {
        ngd_http_close_conn(http);
        return;
    }
    //
    for (;;)
    {
        switch (state)
        {
            case ps_start:
                ...
                state = ps_reqline;
                break;
            case ps_reqline:
                ret = ngd_http_parse_reqline(http);
                //
                if (ret == NGD_OK) {
                    http->smethod.data = http->method_start;
                    http->smethod.len = http->method_end - http->method_start;
                    http->suri.data = http->uri_start;
                    http->suri.len = http->uri_end - http->uri_start;
                    http->sver.data = http->ver_start;
                    http->sver.len = http->ver_end - http->ver_start;
                    state = ps_headers;
                    continue;
                }
                //
                if (ret == NGD_AGAIN) {
                    if (b->last == b->end) {
                        ngd_http_bad_request(http);
                        return;
                    }
                    if (!ngd_conn_on_read(c))
                        return;
                    ret = ngd_conn_recv(c,
                                        b->last,
                                        b->end - b->last,
                                        &n);
                    if (ret == NGD_OK) {
                        b->last += n;
                        continue;
                    }

                    if (ret == NGD_AGAIN) {
                        return;
                    }

                    if (ret == NGD_ERR || ret == NGD_CLOSED) {
                        ngd_http_bad_request(http);
                        return;
                    }
                }

                if (ret == NGD_ERR || ret == NGD_CLOSED) {
                    ngd_http_bad_request(http);
                    return;
                }
                break;
            case ps_headers:
                ret = ngd_http_parse_headers(http);

                if (ret == NGD_OK) {
                    header = ngd_pool_alloc(http->pool, sizeof(*header));
                    if (header == NULL) {
                        ngd_http_bad_request(http);
                        return;
                    }

                    header->key.data = http->key_start;
                    header->key.len = http->key_end - http->key_start;
                    header->value.data = http->value_start;
                    header->value.len = http->value_end - http->value_start;
                    ngd_list_append(http->headers, header);
                    continue;
                }

                if (ret == NGD_HTTP_FULL_HEADER_DONE) {
                    ...

                    if (http->on_chunk) {
                        http->content_length = 0;
                    }

                    state = ps_body;
                    continue;
                }

                if (ret == NGD_ERR || ret == NGD_CLOSED) {
                    ngd_http_bad_request(http);
                    return;
                }

                if (ret == NGD_AGAIN) {
                    if (b->last == b->end) {
                        ngd_http_bad_request(http);
                        return;
                    }

                    if (!ngd_conn_on_read(c))
                        return;

                    ret = ngd_conn_recv(c,
                                        b->last,
                                        b->end - b->last,
                                        &n);
                    if (ret == NGD_OK) {
                        b->last += n;
                        continue;
                    }

                    if (ret == NGD_AGAIN)
                        return;

                    ngd_http_bad_request(http);
                    return;
                }
            case ps_body:
                if (http->on_chunk) {
                    state = ps_body_chunk;
                    continue;
                }

                if (http->content_length == 0) {
                    state = ps_build_resp;
                    continue;
                }

                if (http->content_length > NGD_HTTP_LIMIT_BODY) {
                    ngd_http_bad_request(http);
                    return;
                }

                if (http->content_length <= NGD_HTTP_INBUF_SIZE) {
                        http->state = ps_body_len;
                        continue;
                } else {
                    http->fd_temp = ngd_http_create_temp_file();
                    if (http->fd_temp < 0) {
                        ngd_http_bad_request(http);
                        return;
                    }

                    http->on_body_file = true;
                    http->offset = 0;
                    state = ps_body_len;
                    continue;
                }
        }

    }
}
