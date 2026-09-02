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
    //
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
int
ngd_http_handle_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    enum {
        ps_start=NGD_STATE_START,
        ps_reqline,
        ps_headers,
        ps_body,
        ps_body_chunk,
        ps_body_len,
        ps_build_resp,
        ps_compress_resp,
        ps_send_resp,
    } state;
    int ret;
    ngd_buf_t *b;
    size_t n;
    ngd_http_header_t *header;
    ngd_str_t *key;
    //
    http = ngd_conn_get_data(c);
    state = http->state;
    b = http->inbuf;
    ret = NGD_AGAIN;
    //
    for (;;)
    {
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
                    ret = ngd_http_read_request(http)
                    if (ret == NGD_AGAIN) {
                        return;
                    }

                    if (ret == NGD_OK) {

                    }

                    if (ret == NGD_ERR) {
                        ngd_http_close_conn();
                        return;
                    }
                    break;
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
                if (ret == NGD_AGAIN) {

                }

                if (ret == NGD_OK) {
                    header = pool_alloc(http->pool, sizeof(*header));
                    header->key.data = http->key_start;
                    header->key.len = http->key_end - http->key_start;
                    header->value.data = http->value_start;
                    header->value.len = http->value_end - http->value_start;
                    ngd_list_append(http->headers, (void *)header);
                }
                if (ret == NGD_HTTP_FULL_HEADER_DONE) {
                    for (ngd_list_node_t *node = http->headers.head;
                                          node != NULL;
                                          node->next;)
                    {
                        header = node->data;
                        if (ngd_str_iequal(header->key, NGD_STR_C("Content-Length"))) {
                            ret = ngd_str_to_size(header->key, &http->content_length);
                            if (ret == NGD_ERR)
                                ngd_http_close_conn(http);
                        }

                        if (ngd_str_iequal(header->key ,NGD_STR_C("Transfer-Encoding"))) {
                            if (ngd_str_iequal(header->value, NGD_STR_C("chunked"))) {
                                http->on_chunked = true;
                            }
                        }

                        if (ngd_str_iequal(header->key ,NGD_STR_C("Connection"))) {
                            if (ngd_str_iequal(header->value, NGD_STR_C("keep-alive"))) {
                                http->on_keep_alive = true;
                            }
                        }

                        if (ngd_str_iequal(header->key ,NGD_STR_C("Accept-Encoding"))) {
                            if (ngd_str_isin(NGD_STR_C("gzip"), header->value, false)) {
                                http->on_gzip = true;
                            } else {
                                http->on_gzip = false;
                            }
                        }
                        state = ps_body;
                        continue;
                    }
                }
                if (ret == NGD_ERR) {

                }
                break;
            case ps_body:
                if (http->on_chunk)
                    state = ps_body_chunk;
                else {
                    state = ps_body_len;
                    if (http->content_length > NGD_HTTP_LIMIT_BODY) {
                        ngd_http_close_conn(http);
                        return;
                    }
                }
                break;
            case ps_body_chunk:
                ret = ngd_http_parse_chunk(http);
                if (ret == NGD_OK) {
                    state = ps_build_resp;
                }
                break;
            case ps_body_len:
                if (http->recved_each <= http->content_length) {
                    //consume until last
                    //read request
                    //consume until last
                    //if last == end
                    ret = ngd_http_read_request(http);
                    if (ret == NGD_AGAIN)
                        return;
                    if (ret == NGD_ERR)
                        ngd_http_close_conn(http);
                    //read request full;
                    //save to temp file
                    //read request full
                    //save to temp file
                }
                //
                ret = ngd_http_read_request(http);
                if (ret == NGD_AGAIN) {
                    return;
                }

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
// }
