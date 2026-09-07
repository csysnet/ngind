#include "ngd_http.h"
//
void
ngd_http_init_conn(ngd_conn_t *c)
{
    ngd_pool_t *pool;
    ngd_pool_t *pool_req;
    ngd_http_t *http;
    void *buf;
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
    http->pool = pool;
    //
    http->pool_req = ngd_pool_create();
    if (http->pool_req == NULL) {
        ngd_pool_destroy(http->pool);
        ngd_conn_close(c);
        return;
    }
    //
    buf = ngd_pool_alloc(pool, NGD_HTTP_BUFLEN);
    if (buf == NULL) {
        ngd_pool_destroy(http->pool);
        ngd_pool_destroy(http->pool_req);
        ngd_conn_close(c);
        return;
    }
    ngd_buf_init(&http->inbuf, buf, NGD_HTTP_BUFLEN);
    //
    buf = ngd_pool_alloc(pool, NGD_HTTP_BUFLEN);
    if (buf == NULL) {
        ngd_pool_destroy(http->pool);
        ngd_pool_destroy(http->pool_req);
        ngd_conn_close(c);
        return;
    }
    ngd_buf_init(&http->outbuf, buf, NGD_HTTP_BUFLEN);
    //
    if (ngd_conn_init(
            c,
            ngd_http_handle_conn,
            http,
            NGD_HTTP_TIMEOUT_READ
        ) == NGD_ERR)
    {
        ngd_pool_destroy(http->pool);
        ngd_pool_destroy(http->pool_req);
        ngd_conn_close(c);
        return;
    }
    //
    http->conn = c;
    http->state = NGD_STATE_START;
}
void
ngd_http_close_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    //
    http = ngd_conn_get_data(c);
    //
    ngd_pool_destroy(http->pool);
    ngd_pool_destroy(http->pool_req);
    ngd_conn_close(c);
}
void
ngd_http_handle_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    enum {
        ps_start=NGD_STATE_START,
        ps_reqline,
        ps_headers,
        ps_build_resp,
        ps_send_resp
    } state;
    int ret;
    ngd_buf_t *b;
    size_t n;
    ngd_http_header_t *header;
    //
    http = ngd_conn_get_data(c);
    state = http->state;
    b = &http->inbuf;
    //
    if (ngd_conn_on_timeout(c))
        goto error;
    //
    for (;;)
    {
        switch (state) {
            case ps_start:
                ngd_list_init(&http->headers, http->pool_req);
                http->on_content_length = false;
                http->on_chunk = false;
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
                    ret = ngd_conn_recv(c, b->last, b->end - b->last, &n);
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
                //
                if (!ngd_str_equal(http->suri, NGD_STR_C("GET")))
                    goto error;
                //
                state = ps_headers;
                break;
            case ps_headers:
                ret = ngd_http_parse_headers(http);
                if (ret == NGD_ERR)
                    goto error;
                if (ret == NGD_AGAIN) {
                    if (b->last == b->end)
                        goto error;
                    ret = ngd_conn_recv(c, b->last, b->end - b->last, &n);
                    if (ret == NGD_ERR)
                        goto error;
                    if (ret == NGD_AGAIN)
                        goto again;
                    b->last += n;
                    break;
                }
                //
                if (ret == NGD_HTTP_FULL_HEADER_DONE) {
                    for (ngd_list_node_t *node = http->headers.head;
                                          node != NULL;
                                          node = node->next)
                    {
                        header = node->data;
                        if (ngd_str_iequal(header->key, NGD_STR_C("Content-Length"))) {
                            http->on_content_length = true;
                            continue;
                        }
                        if (ngd_str_iequal(header->key ,NGD_STR_C("Transfer-Encoding"))) {
                            if (!ngd_str_iequal(header->value, NGD_STR_C("chunked")))
                                goto error;
                            http->on_chunk = true;
                            continue;
                        }
                        if (ngd_str_iequal(header->key ,NGD_STR_C("Connection"))) {
                            if (ngd_str_iequal(header->value, NGD_STR_C("keep-alive"))) {
                                http->on_keep_alive = true;
                                continue;
                            }
                            if (ngd_str_iequal(header->value, NGD_STR_C("close"))) {
                                http->on_keep_alive = false;
                                continue;
                            }
                            goto error;
                        }
                    }

                    if (http->on_chunk || http->on_content_length)
                        goto error;
                    state = ps_build_resp;
                    break;
                }
                header = ngd_pool_alloc(http->pool_req, sizeof(*header));
                if (header == NULL)
                    goto error;
                header->key.data = http->key_start;
                header->key.len = http->key_end - http->key_start;
                header->value.data = http->value_start;
                header->value.len = http->value_end - http->value_start;
                ngd_list_append(&http->headers, (void *)header);
                break;
            case ps_build_resp:
                if (ngd_http_build_resp(http) == NGD_ERR)
                    goto error;
                ret = ngd_http_build_resp(http);
                if (ret == NGD_ERR)
                    goto error;
                state = ps_send_resp;
                break;
            case ps_send_resp:
                if (ngd_conn_on_read(http->conn))
                    goto again;
                //
                ret = ngd_http_send_resp(http);
                if (ret == NGD_ERR)
                    goto error;
                if (ret == NGD_AGAIN)
                    goto again;
                if (ret == NGD_HTTP_FULL_SEND_DONE) {
                    ngd_file_close(&http->file_send);
                    if (!http->on_keep_alive) {
                        goto done;
                    }
                    //
                    ngd_pool_destroy(http->pool_req);
                    http->pool_req = ngd_pool_create();
                    if (http->pool_req == NULL)
                        goto error;
                    //
                    n = b->last - b->pos;
                    ngd_str_cpy(b->start, b->pos, n);
                    b->last += n;
                    //
                    state = ps_start;
                }
                break;
        }
    }
done:
    ngd_http_close_conn(http->conn);
    return;
again:
    http->state = state;
    return;
error:
    ngd_http_close_conn(http->conn);
    return;
}
//
int
ngd_http_send_resp(ngd_http_t *http)
{
    ngd_buf_t *b;
    size_t n;
    int ret;
    //
    b = &http->outbuf;
    //
    if (b->pos < b->last) {
        ret = ngd_conn_send(
            http->conn,
            b->pos,
            b->last - b->pos,
            &n
        );

        if (ret != NGD_OK)
            return ret;

        b->pos += n;

        return NGD_OK;
    }

    b->pos = b->start;
    b->last = b->start;

    ret = ngd_file_read(
        &http->file_send,
        b->last,
        b->end - b->last,
        &n
    );

    if (ret == NGD_FILE_DONE)
        return NGD_HTTP_FULL_SEND_DONE;

    if (ret == NGD_ERR)
        return NGD_ERR;

    b->last += n;

    return NGD_OK;
}

//
int
ngd_http_build_resp(ngd_http_t *http)
{
    ngd_buf_t *b;
    const char *vconn;
    const char *vtype;
    char *file_path;
    size_t len;
    size_t bytes_written;
    //
    b = &http->outbuf;
    //
    if (http->on_keep_alive)
        vconn = "keep-alive";
    else
        vconn = "close";
    //
    if (ngd_str_isin(NGD_STR_C(".html"), http->suri))
        vtype = "text/html";
    else if (ngd_str_isin(NGD_STR_C(".css"), http->suri))
        vtype = "text/css";
    else if (ngd_str_isin(NGD_STR_C(".js"), http->suri))
        vtype = "text/javascript";
    else
        return NGD_ERR;
    //
    file_path = ngd_pool_alloc(http->pool_req, (sizeof(NGD_STATIC_PATH) - 1) + (http->suri.len) + 1);
    ngd_str_cpy(file_path, NGD_STATIC_PATH, (sizeof(NGD_STATIC_PATH) - 1) );
    ngd_str_cpy(file_path, http->suri.data, http->suri.len);
    file_path[(sizeof(NGD_STATIC_PATH) - 1) + (http->suri.len)] = '\0';
    //
    ngd_file_init(&http->file_send);
    if (ngd_file_open(&http->file_send, file_path) == NGD_ERR)
        return NGD_ERR;
    if (ngd_file_get_size(http->file_send, &len) == NGD_ERR)
        return NGD_ERR;
    //
    if (ngd_str_snprintf(
            b->last,
            b->end - b->last,
            &bytes_written,
            "HTTP/1.1 200 OK\r\n"
            "Connection: %s\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %zu\r\n"
            "\r\n",
            vconn,
            vtype,
            len)
        == NGD_ERR)
    {
        return NGD_ERR;
    }

    b->last += bytes_written;

    return NGD_OK;
}
//
int
ngd_http_parse_reqline(ngd_http_t *http)
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
    //
    state = http->state_req;
    b = &http->inbuf;
    //
    for (p = b->pos; p < b->last; p++)
    {
        switch (state)
        {
            case ps_start:
                http->method_start = p;
                state = ps_method;
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
                if (*p == '\n') {
                    http->ver_end = p - 1;
                    goto done;
                }
                break;
        }
    }
    b->pos = b->last;
    http->state_req = state;
    return NGD_AGAIN;
done:
    b->pos = p + 1;
    http->state_req = ps_start;
    return NGD_OK;
}
//
int
ngd_http_parse_headers(ngd_http_t *http)
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
    state = http->state_req;
    b = &http->inbuf;
    //
    for (p = b->pos; p < b->last; p++)
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
    http->state_req = state;
    return NGD_AGAIN;
done:
    b->pos = p + 1;
    http->state_req = ps_start;
    return NGD_OK;
header_done:
    b->pos = p + 1;
    http->state_req = ps_start;
    return NGD_HTTP_FULL_HEADER_DONE;
}
//
