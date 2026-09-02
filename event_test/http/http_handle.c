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
    size_t bytes_recved;
    void *new_buf;
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
                http->bytes_recved = 0;
                ret = ngd_conn_recv(c, c->inbuf, b->last - b->end, &bytes_recved);
                if (ret == NGD_OK) {
                    b->last += bytes_recved;
                    http->recved_each += bytes_recved;
                }
                if (ret == NGD_ERR || ret == NGD_CLOSED) {
                    ngd_http_close_conn();
                    return;
                }
                state = ps_reqline;
                break;
            case ps_reqline:
                ret = ngd_http_parse_reqline(http);
                //
                if (ret == NGD_AGAIN) {
                    if (http->recved_each > NGD_HTTP_LIMIT_REQLINE) {
                        ngd_http_close_conn(c);
                        return;
                    }
                    if (c->last == b->end) {
                        new_buf = pool_alloc(http->pool, NGD_HTTP_INBUF_MEDIUM);
                        //
                        ngd_str_cpy(NGD_STR(b->start, b->end - b->start), NGD_STR(newbuf, NGD_HTTP_INBUF_MEDIUM))
                        b->last = new_buf + (b->last - b->start);
                        b->pos = new_buf + (b->pos - b->start);
                        b->start = new_buf;
                        b->end = new_buf + NGD_HTTP_INBUF_MEDIUM;
                        //
                        http->ver_start = new_buf + (http->ver_start - http->method_start)
                        http->uri_end = new_buf + (http->uri_end - http->method_start)
                        http->uri_start = new_buf + (http->uri_start - http->method_start)
                        http->method_end = new_buf + (http->method_end - http->method_start)
                        http->method_start = new_buf;
                        //
                    }

                    ret = ngd_conn_recv(c, c->inbuf, b->last - b->end, &bytes_recved);
                    if (ret == NGD_OK) {
                        b->last += bytes_recved;
                        http->recved_each += bytes_recved;
                    }
                    if (ret == NGD_AGAIN) {
                        return;
                    }
                    if (ret == NGD_ERR || ret == NGD_CLOSED) {
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
                ret = ngd_http_parse_headers(http);
                if (ret == NGD_AGAIN) {
                    if (http->recved_each > NGD_HTTP_LIMIT_REQLINE) {
                        ngd_http_close_conn(c);
                        return;
                    }

                    if (b->last == b->end) {
                        switch (b->end - b->start)
                        {
                            case NGD_HTTP_INBUF_SMALL:
                                n = NGD_HTTP_INBUF_MEDIUM; break;
                            case NGD_HTTP_INBUF_MEDIUM:
                                n = NGD_HTTP_INBUF_LARGE; break;
                        }
                        new_buf = pool_alloc(http->pool, n);
                        ngd_str_cpy(NGD_STR(http->key_start, b->last), NGD_STR(new_buf, n));
                        //
                        b->last = new_buf + (b->last - http->key_start);
                        b->pos = new_buf + (b->pos - b->key_start);
                        b->start = new_buf;
                        b->end = new_buf + NGD_HTTP_INBUF_MEDIUM;
                        //
                        http->value_start = new_buf + (http->value_start - http->key_start);
                        http->key_end = new_buf + (http->key_end - http->key_start);
                        http->key_start = new_buf;
                        //
                    }
                    ret = ngd_conn_recv(c, b, b->end - b->last, &bytes_recved);
                    if (ret == NGD_OK) {
                        b->last += bytes_recved;
                        http->recved_each += bytes_recved;
                    }

                    if (ret == NGD_ERR) {
                        ngd_http_close_conn(c);
                        return;
                    }

                    if (ret == NGD_AGAIN)
                        return;
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
                    ngd_http_close_conn(http);
                    return;
                }
                break;
            case ps_body:
                if (http->on_chunk) {
                    new_buf = pool_alloc(http->pool, NGD_HTTP_INBUF_LARGE);
                    b->start = new_buf;
                    b->pos = b->start;
                    b->last = b->start;
                    b->end = b->start + NGD_HTTP_INBUF_LARGE;
                    state = ps_body_chunk;
                } else {
                    if (http->content_length == 0 ||
                        http->content_length > NGD_HTTP_LIMIT_BODY) {
                        ngd_http_close_conn(http);
                        return;
                    }
                    if (b->end - b->pos <= http->content_length) {
                        b->start = b->pos;
                    } else {
                        if (http->content_length > NGD_HTTP_INBUF_LARGE) {
                            http->on_body_file = true;
                            fd_temp = open()...
                            n = NGD_HTTP_INBUF_LARGE;
                        } else if (http->content_length > NGD_HTTP_INBUF_MEDIUM) {
                            n = NGD_HTTP_INBUF_LARGE;
                        } else if (http->content_length > NGD_HTTP_INBUF_SMALL) {
                            n = NGD_HTTP_INBUF_MEDIUM
                        } else {
                            n = NGD_HTTP_INBUF_SMALL;
                        }
                        new_buf = pool_alloc(http->pool, n);
                        b->start = new_buf;
                        b->pos = b->start;
                        b->last = b->start;
                        b->end = b->start + n;
                    }
                    state = ps_body_len;
                }
                break;
            case ps_body_chunk:
                ret = ngd_http_parse_chunk(http);
                if (ret == NGD_OK) {
                    state = ps_build_resp;
                }
                if (ret == NGD_AGAIN) {
                    if (http->recved_each > NGD_HTTP_LIMIT_BODY) {
                        ngd_http_close_conn(http);
                        return;
                    }
                    if (b->end == b->last) {

                        if (http->on_body_file) {
                            write(fd_temp, b->start, b->end - b->start);
                            b->pos = b->start;
                            b->last = b->start;
                        } else {

                        }
                    }
                    ret = ngd_conn_recv(c, b, b->end - b->last, &bytes_recved)
                    if (ret == NGD_OK) {
                        b->last += bytes_recved;
                        http->recved_each += bytes_recved;
                    }
                    if (ret == NGD_AGAIN) {
                        return;
                    }

                    if (ret == NGD_ERR) {
                        ngd_http_close_conn(c);
                        return;
                    }
                }
                break;
            case ps_body_len:
                if ()
                if (http->recved_each <= http->content_length) {
                    //consume until last
                    //read request
                    //consume until last
                    //if last == end
                    ret = ngd_conn_recv(c, b, b->end - b->last, &bytes_recved);
                    if (ret == NGD_OK) {
                        http->recved_each += bytes_recved;
                        b->last += bytes_recved;
                    }
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
// }
