#include "ngd_http.h"
//
int
ngd_http_read_request(ngd_http_t *c)
{
    size_t bytes_recved;
    ngd_http_t *http;
    ngd_buf_t *b;
    int ret;
    void *p;
    //
    http = c->data;
    b = http->inbuf;
    if (b->last == b->end) {
        if (!max_mode) {
            alloc larger
            if (http->bytes_recved == NGD_HTTP_INBUF_SMALL) {
                p = pool_alloc(http->pool, NGD_HTTP_INBUF_MEDIUM);
                ngd_str_cpy(b->pos, b->last - b->pos, p, NGD_HTTP_INBUF_MEDIUM);
                ngd_buf_init(b, p, NGD_HTTP_INBUF_MEDIUM)
            } else if (http->bytes_recved == NGD_HTTP_INBUF_MEDIUM) {
                copy avaible space, alloc equal large
            } else if (http->bytes_recved == NGD_HTTP_INBUF_LARGE)

                return NGD_ERR;
        }
    }
    else {
        ret = ngd_conn_recv(c, b->last, b->end - b->last, &bytes_recved);
        if (ret == NGD_OK) {
            b->last += n;
            rev->recved_each += n;
        }

        if (ret == NGD_AGAIN) {

        }
    }
    //

}


int
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
//
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
    int ret_read;
    size_t bytes_recved;
    void *data;
    ngd_buf_t *b;
    //
    http = ngd_conn_get_data(c);
    state = http->state;
    b = http->inbuf;
    ngd_http_b
    //
    for (;;)
    {
        if (ret == NGD_AGAIN) {
            ret = ngd_conn_recv(c, b->last, b->end - b->last, &bytes_recved);
            if (ret == NGD_AGAIN)
                return;
            if (ret == NGD_OK) {
                if (b->last == )
            }

        }
        if (ret == NGD_AGAIN) {
            if (buf->last == buf->end) {
                data = ngd_pool_alloc(http->pool, NGD_)

                pool_alloc(1024)
                ngd_str_cpy(b->pos, b->last - b->pos, )
            } else {
                n = b->last - b->pos;
                if (n == 0) {
                    n = c->recv(c, b->last, b->end - b->last);

                }
                ret = ngd_conn_recv(c, b->last, b->end - b->last, &n);
                if (ret == NGD_OK) {
                    b->pos = b->last;
                    b->last += b->end - b->last;
                }
                if (ret == NGD_AGAIN) {
                    b->last += n;
                }
                if (ret == NGD_ERR) {
                    ngd_http_close_conn(c);
                }
            }
        }
        switch (state)
        {
            case ps_start:
                http->state_parse = NGD_STATE_START;
                http->state_each = 0;
                break;
            case ps_reqline:
                ret = ngd_http_handle_reqline(http);
                //
                if (ret == NGD_AGAIN) {
                    ret_read = ngd_http_read_request(http);
                    if (ret_read == NGD_AGAIN)
                        return;
                    if (ret_read == NGD_ERR) {
                        ngd_http_close_conn(c);
                    }
                    if (http->recved_each > NGD_HTTP_LIMIT_REQLINE) {
                        ngd_http_close_conn(c);
                    }
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
                    ngd_http_close_conn(c);
                    return;
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
// int
// ngd_http_handle_conn(ngd_conn_t *c)
// {
//     ngd_http_t *http;
//     enum {
//         ps_start=NGD_STATE_START,
//         ps_reqline,
//         ps_headers,
//         ps_body,
//         ps_build_resp,
//         ps_compress_resp,
//         ps_send_resp,
//     } state;
//     int ret;
//     ngd_buf_t *b;
//     //
//     http = ngd_conn_get_data(c);
//     state = http->state;
//     b = http->inbuf;
//     //
//     ret = ngd_http_read_request(c);
//     if (ret == NGD_AGAIN) {
//         return;
//     }
//     if (ret == NGD_OK);
//     if (ret == NGD_ERR) {
//         ngd_http_close_conn(c);
//     }
//     //
//     for (;;)
//     {
//         switch (state)
//         {
//             case ps_start:
//                 http->state_parse = NGD_STATE_START;
//                 http->state_each = 0;
//                 break;
//             case ps_reqline:
//                 ret = ngd_http_handle_reqline(http);
//                 //
//                 if (ret == NGD_AGAIN) {
//                     if (http->recved_each > NGD_HTTP_LIMIT_REQLINE) {
//                         ngd_http_close_conn(c);
//                     }
//                     ret = ngd_http_read_request(http);
//                     if (ret == NGD_AGAIN) {
//                         return;
//                     }
//                     if (ret == NGD_OK) {
//                         continue;
//                     }

//                     if (ret == NGD_ERR) {
//                         ngd_http_close_conn(http);
//                     }
//                 }
//                 //
//                 if (ret == NGD_OK) {
//                     http->smethod.data = http->method_start;
//                     http->smethod.len = http->method_end - http->method_start;
//                     http->suri.data = http->uri_start;
//                     http->suri.len = http->uri_end - http->uri_start;
//                     http->sver.data = http->ver_start;
//                     http->sver.len = http->ver_end - http->ver_start;
//                     http->recved_each += n;
//                     http->state_each = 0;
//                     state = ps_headers;
//                 }

//                 if (ret == NGD_ERR) {

//                 }
//                 break;
//             case ps_headers:
//                 ret = ngd_http_handle_headers(http);
//                 if (ret == NGD_OK) {
//                 }
//                 if (ret == NGD_AGAIN) {

//                 }
//                 if (ret == NGD_ERR) {

//                 }
//                 break;
//             case ps_body:
//                 ret = ngd_http_handle_body(http);
//                 break;
//             case ps_build_resp:
//                 ret = ngd_http_build_resp(http);
//                 break;
//             case ps_compress_resp:
//                 ret = ngd_http_compress_resp(http);
//                 break;
//             case ps_send_resp:
//                 ret = ngd_http_send_resp(http);
//                 break;
//         }
//     }

//     //
//     return NGD_OK;
// }
//
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
    //
    http = ngd_conn_get_data(c);
    state = http->state;
    b = http->inbuf;
    //
    for (;;)
    {
        ret = ngd_http_read_request(c);
        if (ret == NGD_AGAIN) {
            return;
        }
        if (ret == NGD_ERR) {
            ngd_http_close_conn(http);
        }

        if (ret == NGD_OK) {
            switch (state)
            {
                case ps_start:
                    http->state_parse = NGD_STATE_START;
                    http->state_each = 0;
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
                        ngd_map_init(http->headers, http->pool);
                        state = ps_headers;
                    }

                    if (ret == NGD_ERR) {

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
        }
    }

    //
    return NGD_OK;
// }
//
//
int ngd_http_close_conn(ngd_conn_t *c);
