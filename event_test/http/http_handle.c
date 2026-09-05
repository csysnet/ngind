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
void
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
        ps_send_resp,
    } state;
    int ret;
    ngd_buf_t *hb;
    ngd_buf_t *bb;
    ngd_buf_t *ob;
    size_t n;
    ngd_http_header_t *header;
    //
    http = ngd_conn_get_data(c);
    state = http->state;
    hb = http->headbuf;
    bb = http->bodybuf;
    ob = http->outbuf;
    //
    for (;;)
    {
        switch (state)
        {
            case ps_start:
                ret = ngd_conn_recv(c, c->inbuf, b->last - b->end, &n);
                if (ret == NGD_OK) {
                    b->last += bytes_recved;
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
                    if (b->last == b->end) {
                        ngd_http_close_conn();
                        return;
                    }
                    //
                    ret = ngd_conn_recv(c, b, b->last - b->end, &n);
                    if (ret == NGD_OK) {
                        b->last += n;
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
                    state = ps_headers;
                }

                if (ret == NGD_ERR) {
                    ngd_http_close_conn(http);
                    return;
                }
                break;
            case ps_headers:
                ret = ngd_http_parse_headers(http);
                if (ret == NGD_ERR) {
                    ngd_http_close_conn(http);
                    return;
                }
                if (ret == NGD_AGAIN) {
                    if (b->last == b->end) {
                        ngd_http_close_conn(http);
                        return;
                    }
                    ret = ngd_conn_recv(c, b, b->end - b->last, &n);
                    if (ret == NGD_OK) {
                        b->last += bytes_recved;
                    }

                    if (ret == NGD_ERR || ret == NGD_CLOSED) {
                        ngd_http_close_conn(c);
                        return;
                    }

                    if (ret == NGD_AGAIN)
                        return;
                    break;
                }
                //
                if (ret == NGD_OK) {
                    header = pool_alloc(http->pool, sizeof(*header));
                    header->key.data = http->key_start;
                    header->key.len = http->key_end - http->key_start;
                    header->value.data = http->value_start;
                    header->value.len = http->value_end - http->value_start;
                    ngd_list_append(http->headers, (void *)header);
                    break;
                }
                if (ret == NGD_HTTP_FULL_HEADER_DONE) {
                    for (ngd_list_node_t *node = http->headers.head;
                                          node != NULL;
                                          node = node->next;)
                    {
                        header = node->data;
                        if (ngd_str_iequal(header->key, NGD_STR_C("Content-Length"))) {
                            http->on_content_length = true;
                            ret = ngd_str_to_size(header->key, &http->content_length);
                            if (ret == NGD_ERR) {
                                ngd_http_close_conn(http);
                                return;
                            }
                        } else if (ngd_str_iequal(header->key ,NGD_STR_C("Transfer-Encoding"))) {
                            if (ngd_str_iequal(header->value, NGD_STR_C("chunked"))) {
                                http->on_chunked = true;
                            }
                        } else if (ngd_str_iequal(header->key ,NGD_STR_C("Connection"))) {
                            if (ngd_str_iequal(header->value, NGD_STR_C("keep-alive"))) {
                                http->on_keep_alive = true;
                            }
                        }
                    }
                    state = ps_body;
                    break;
                }
                break;
            case ps_body:
                if (http->on_chunk) {
                    state = ps_body_chunk;
                } else {
                    if (!http->on_content_length ||
                        http->content_length > NGD_HTTP_LIMIT_BODY) {
                        ngd_http_close_conn(http);
                        return;
                    }
                    if (http->content_length == 0) {
                        state = ps_build_resp;
                        break;
                    }
                    //
                    if (http->content_length > NGD_HTTP_BODY_LEN) {
                        http->on_body_file = true;
                        http->temp_fd = ..
                        fd_temp = open()...
                    }
                    state = ps_body_len;
                }
                ngd_str_cpy(
                    hb->pos,
                    bb->pos,
                    (size_t)hb->last - hb->pos
                );
                bb->last += hb->last - hb->pos;
                http->body_recved += hb->last - hb->pos;
                break;
            case ps_body_chunk:
                ret = ngd_http_parse_chunk(http);
                if (ret == NGD_ERR) {
                    ngd_http_close_conn(http);
                    return;
                }
                if (ret == NGD_OK) {
                    state = ps_build_resp;
                    break;
                }
                if (ret == NGD_AGAIN) {
                    if (http->body_recved > NGD_HTTP_LIMIT_BODY) {
                        ngd_http_close_conn(http);
                        return;
                    }
                    //
                    if (bb->last == bb->end) {
                        if (!http->on_body_file) {
                            ngd_http_close_conn(c);
                            return;
                        }
                        ret = ngd_file_write(http->fd_temp, bb->start, bb->end - bb->start);
                        if (ret == NGD_ERR) {
                            ngd_http_close_conn(http);
                            return;
                        }
                        bb->pos = bb->start;
                        bb->last = bb->start;
                    }
                    ret = ngd_conn_recv(c, bb, bb->end - bb->last, &n);
                    if (ret == NGD_OK) {
                        bb->last += n;
                        bb->body_recved += n;
                        break;
                    }
                    if (ret == NGD_ERR) {
                        ngd_http_close_conn(c);
                        return;
                    }
                    if (ret == NGD_AGAIN)
                        return;

                }
                break;
            case ps_body_len:
                if (bb->body_recved == http->content_length) {
                    state = ps_build_resp;
                    break;
                }
                if (bb->last == bb->end) {
                    if (!http->on_body_file) {
                        state = ps_build_resp;
                        break;
                    }
                    ret = ngd_file_write(http->fd_temp, bb->start, bb->end - bb->start);
                    if (ret == NGD_ERR) {
                        ngd_http_close_conn(http);
                        return;
                    }
                    bb->pos = bb->start;
                    bb->last = bb->start;
                }
                ret = ngd_conn_recv(c, bb, b->end - bb->last, &n);
                if (ret == NGD_ERR) {
                    ngd_http_close_conn(c);
                    return;
                }
                if (ret == NGD_AGAIN)
                    return;
                if (ret == NGD_OK) {
                    bb->last += n;
                    bb->body_recved += n;
                    break;
                }
                break;
            case ps_build_resp:
                ret = ngd_http_build_resp(http);
                break;
            case ps_send_resp:
                ret = ngd_http_send_resp(http);
                break;
        }
    }
    http->state = state;
    //
    return NGD_OK;
}
//
int
ngd_http_build_resp(ngd_http_t *http)
{
    ngd_buf_t *ob;
    char *stype;
    char *sconn;
    size_t *len;
    //
    const char *s =
        "HTTP/1.1 200 OK\r\n"
        "Connection: %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %lu\r\n"
        "\r\n"
    ob = http->outbuf;
    //Connection
    if (http->on_keep_alive)
        sconn = "keep-alive";
    else
        sconn = "close";
    //Content-Type
    if (ngd_str_isin(NGD_STR_C(".html"), http->uri)) {
        stype = "text/html";
    } else if (ngd_str_isin(NGD_STR_C(".css"), http->uri)) {
        stype = "text/css";
    } else if (ngd_str_isin(NGD_STR_C(".js"), http->uri)) {
        stype = "text/javascript";

    } else if (ngd_str_isin(NGD_STR_C(".json"), http->uri)) {
        stype = "application/json";

    } else if (ngd_str_isin(NGD_STR_C(".jpg"), http->uri) ||
               ngd_str_isin(NGD_STR_C(".jpeg"), http->uri)) {
        stype = "image/jpeg";

    } else if (ngd_str_isin(NGD_STR_C(".png"), http->uri)) {
        stype = "image/png";

    } else if (ngd_str_isin(NGD_STR_C(".gif"), http->uri)) {
        stype = "image/gif";

    } else if (ngd_str_isin(NGD_STR_C(".svg"), http->uri)) {
        stype = "image/svg+xml";

    } else if (ngd_str_isin(NGD_STR_C(".ico"), http->uri)) {
        stype = "image/x-icon";
    } else {
        stype = "application/octet-stream";
    }
    //Content-Length
    ngd_file_init(http->file_send);
    if (ngd_file_open(http->file_send, ...) == NGD_ERR)
        return NGD_ERR;
    ngd_file_get_size(http->file_send, &len);
    len += (sizeof(stype) - 1);
    //
    remain = NGD_HTTP_OUTBUF_LEN - len;
    ngd_file_read(http->file_send, http->onbuf, remain, &n);
    write remain to outbuf, remain = 16kb - sizeof(s); ok and return to
    //
}
//
void
ngd_http_init_conn(ngd_conn_t *c)
{
    ngd_pool_t *pool;
    ngd_http_t *http;
    //
    pool = ngd_pool_create();
    //
    http = ngd_pool_alloc(pool, sizeof(ngd_http_t));
    http->pool = pool;
    http->state = NGD_STATE_START;
    http->state_parse = NGD_STATE_START;
    //
    ngd_buf_init(&http->inbuf,
                 pool_alloc(http->pool, NGD_HTTP_INBUF_SMALL),
                 NGD_HTTP_INBUF_SMALL);
    //
    ngd_conn_init(c,
                  ngd_http_handle_conn,
                  (void *)http,
                  60000);//60s
    //
    return NGD_OK;
}
