#include "ngd_http.h"
//
void
ngd_http_init_conn(ngd_conn_t *c)
{
    ngd_pool_t *pool;
    ngd_pool_t *pool_req;
    ngd_http_t *http;
    void *buf;
    int ret;
    //
    pool = ngd_pool_create();
    pool_req = ngd_pool_create();
    //
    http = ngd_pool_alloc(pool, sizeof(ngd_http_t));
    http->pool = pool;
    http->pool_req = pool_req;
    http->state = NGD_STATE_START;
    http->state_req = NGD_STATE_START;
    //go to start
    ngd_file_init(http->file_temp);
    ngd_file_init(http->file_send);
    //
    buf = pool_alloc(http->pool, NGD_HTTP_BUFLEN);
    if (buf == NULL) {
        ngd_http_close_conn(http);
        return;
    }
    ngd_buf_init(
        &http->headerbuf,
        pool_alloc(http->pool, NGD_HTTP_BUFLEN),
        NGD_HTTP_BUFLEN
    );
    //
    buf = pool_alloc(http->pool, NGD_HTTP_BUFLEN);
    if (buf == NULL) {
        ngd_http_close_conn(http);
        return;
    }
    ngd_buf_init(
        &http->bodybuf,
        pool_alloc(http->pool, NGD_HTTP_BUFLEN),
        NGD_HTTP_BUFLEN
    );
    //
    buf = pool_alloc(http->pool, NGD_HTTP_BUFLEN);
    if (buf == NULL) {
        ngd_http_close_conn(http);
        return;
    }
    ngd_buf_init(
        &http->outbuf,
        pool_alloc(http->pool, NGD_HTTP_BUFLEN),
        NGD_HTTP_BUFLEN
    );
    //
    ret = ngd_conn_init(
        c,
        ngd_http_handle_conn,
        (void *)http,
        NGD_HTTP_TIMEOUT_INIT
    );
    if (ret == NGD_ERR)
        ngd_http_close_conn(http);

}
//
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
    if (ngd_conn_on_timeout(c))
        goto error;
    //
    for (;;)
    {
        switch (state)
        {
            case ps_start:
                ngd_list_init(http->headers);
                ret = ngd_conn_recv(c, c->inbuf, b->last - b->end, &n);
                if (ret == NGD_ERR)
                    goto error;
                b->last += bytes_recved;
                state = ps_reqline;
                break;
            case ps_reqline:
                ret = ngd_http_parse_reqline(http);
                //
                if (ret == NGD_ERR)
                    goto error;
                if (ret == NGD_AGAIN) {
                    if (b->last == b->end)
                        goto error;
                    //
                    ret = ngd_conn_recv(c, b, b->last - b->end, &n);
                    if (ret == NGD_ERR)
                        goto error;
                    if (ret == NGD_AGAIN)
                        goto again;
                    b->last += n;
                    break;
                }
                //
                http->smethod.data = http->method_start;
                http->smethod.len = http->method_end - http->method_start;
                http->suri.data = http->uri_start;
                http->suri.len = http->uri_end - http->uri_start;
                http->sver.data = http->ver_start;
                http->sver.len = http->ver_end - http->ver_start;
                state = ps_headers;
                break;
            case ps_headers:
                ret = ngd_http_parse_headers(http);
                if (ret == NGD_ERR)
                    goto error;
                if (ret == NGD_AGAIN) {
                    if (hb->last == hb->end)
                        goto error;
                    ret = ngd_conn_recv(c, hb, hb->end - hb->last, &n);
                    if (ret == NGD_ERR)
                        goto error;
                    if (ret == NGD_AGAIN)
                        goto again;
                    hb->last += n;
                    break;
                }
                //
                if (ret == NGD_HTTP_FULL_HEADER_DONE) {
                    for (ngd_list_node_t *node = http->headers.head;
                                          node != NULL;
                                          node = node->next;)
                    {
                        header = node->data;
                        if (ngd_str_iequal(header->key, NGD_STR_C("Content-Length"))) {
                            http->on_content_length = true;
                            ret = ngd_str_to_size(header->key, &http->content_length);
                            if (ret == NGD_ERR)
                                goto error;
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
                header = ngd_pool_alloc(http->pool_req, sizeof(*header));
                header->key.data = http->key_start;
                header->key.len = http->key_end - http->key_start;
                header->value.data = http->value_start;
                header->value.len = http->value_end - http->value_start;
                ngd_list_append(http->headers, (void *)header);
                break;
            case ps_body:
                if (http->on_chunk) {
                    state = ps_body_chunk;
                } else {
                    if (!http->on_content_length || http->content_length > NGD_HTTP_LIMIT_BODY)
                        goto error;
                    if (http->content_length == 0) {
                        state = ps_build_resp;
                        break;
                    }
                    //
                    if (http->content_length > NGD_HTTP_BODY_LEN) {
                        http->on_body_file = true;
                        if (ngd_file_open_temp(&http->file_temp) == NGD_ERR)
                            goto error;
                    }
                    state = ps_body_len;
                }
                ngd_str_cpy(
                    hb->pos,
                    bb->pos,
                    hb->last - hb->pos
                );
                bb->last += hb->last - hb->pos;
                http->body_recved += hb->last - hb->pos;
                break;
            case ps_body_chunk:
                ret = ngd_http_parse_chunk(http);
                if (ret == NGD_ERR)
                    goto error;
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
                state = ps_build_resp;
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
                    if (ret == NGD_ERR)
                        goto error;
                    bb->pos = bb->start;
                    bb->last = bb->start;
                }
                ret = ngd_conn_recv(c, bb, b->end - bb->last, &n);
                if (ret == NGD_ERR)
                    goto error;
                if (ret == NGD_AGAIN)
                    goto again;
                bb->last += n;
                bb->body_recved += n;
                break;
            case ps_build_resp:
                ret = ngd_http_build_resp(http);
                if (ret == NGD_ERR)
                    goto error;
                break;
            case ps_send_resp:
                if (ngd_conn_on_read(http->conn))
                    goto again;
                ret = ngd_http_send_resp(http);
                if (ret == NGD_ERR)
                    goto error;
                if (ret == NGD_AGAIN)
                    goto again;
                if (ret == NGD_HTTP_SEND_FULL_DONE) {
                    ngd_file_close(file->file_temp);
                    ngd_file_close(file->file_send);
                    if (!http->on_keep_alive) {
                        goto done;
                    }
                    ngd_str_cpy(b->pos, b->start, b->last - b->pos);
                    state = ps_start;
                }
                break;
        }
    }
done:
    ngd_http_close_conn(http);
    return;
again:

    http->state = state;
    return;
error:
    ngd_http_close_conn(c);
    http->state = state;
    return;
}
//
