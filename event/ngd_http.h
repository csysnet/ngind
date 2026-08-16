#ifndef NGD_HTTP_CONN_H
#define NGD_HTTP_CONN_H
//contansts
#define NGD_HTTP_FULL_HEADER_DONE 1
#define NGD_HTTP_FULL_CHUNK_DONE 2
//methods
#define NGD_HTTP_GET 1
#define NGD_HTTP_POST 2
#define NGD_HTTP_OPTIONS 3
#define NGD_HTTP_HEAD 4
#define NGD_HTTP_PUT 5
#define NGD_HTTP_PATCH 6
#define NGD_HTTP_DELETE 7
//
typedef struct ngd_http_t ngd_http_t;
typedef struct ngd_req_t ngd_req_t;
//
struct ngd_http_t {
    ngd_conn_t *conn;
    ngd_req_t *req;
    int state;
    int errno;
    //
    pool_t *pool;
    ngd_buf_t *inbuf;
    ngd_buf_t *outbuf;
    //parsed info
    llist_t *headers;
    str_t
    //
};
struct ngd_req_t {
    int state;
    //request line
    str_t *smethod;
    str_t *suri;
    str_t *sver;
    u_char *method_start;
    u_char *method_end;
    u_char *uri_start;
    u_char *uri_end;
    u_char *ver_start;
    u_char *ver_end;
    //header
    u_char *key_start;
    u_char *key_end;
    u_char *value_start;
    u_char *value_end;
    //body
    size_t content_length;
    size_t body_received;
    bool on_chunk;
    size_t chunk_size;
    u_char *chunk_start;
    u_char *chunk_end;
}
struct ngd_parser_reqline {
    u_char *start_method;
    u_char *end_method;
    u_char *start_uri;
    u_char *end_uri;
    u_char *start_ver;
    u_char *end_ver;
}
int
ngd_parser_reqline {
    ngd_parser_reqline *parser,
    ngd_buf_t *b,
    int *state
}
{

}
struct ngd_parser_headerline {
    u_char *key_start;
    u_char *key_end;
    u_char *value_start;
    u_char *value_end;
}

int
p
//
struct ngd_req_t {
    ngd_str_t *smethod, *suri, *sver;
    buf_t *in;
    size_t content_length;
    //
}
//

// ngd_http_handle(ngd_conn_t *c)
// {
//     ngd_http_t *http;
//     enum {
//         proc_reqline,
//         proc_headers,
//         proc_body,
//         proc_build_res,
//         proc_compress_res,
//         proc_send_response
//     } state;
//     int ret;
//     ssize_t n;
//     //
//     http = c->data;
//     state = http->state;
//     //
//     for (;;)
//     {
//         switch (state)
//         {

//             case proc_reqline:
//                 ret = ngd_http_proc_reqline(ngd_req_t *r);
//                 if (ret == NGD_OK) {
//                     state = proc_headers;
//                     break;
//                 }
//                 if (ret == NGD_AGAIN) {
//                     ...
//                 }
//                 if (ret == NGD_ERR) {
//                     ...
//                 }
//                 break;
//             case proc_headers:
//                 ngd_http_proc_headers(ngd_http_t *http);
//                 if (ret == NGD_OK) {
//                     state = proc_h
//                 }
//                 if (ret == NGD_AGAIN) {
//                     ...
//                 }
//                 if (ret == NGD_ERR) {
//                     ...
//                 }
//                 break;
//             case proc_body:
//                 ngd_http_proc_body(ngd_http_t *http);
//                 break;
//             case proc_build_resp:
//                 ngd_proc_build_resp(ngd_http_t *http);
//                 break;
//             case proc_compress_resp:
//                 ngd_proc_compress_resp(ngd_http_t *http);
//                 break;
//             case proc_send_resp:
//                 ngd_proc_send_resp(ngd_http_t *http);
//                 break;
//         }
//     }



//     if (c->on_read) read();
//     if (c->on_write) ..

//     // if (c->on_timeout) ..
// }
ngd_http_handle(ngd_conn_t *c)
{
    ngd_http_t *http;
    enum {
        proc_reqline,
        proc_headers,
        proc_body,
        proc_build_res,
        proc_compress_res,
        proc_send_response
    } state;
    int ret;
    ssize_t n;
    //
    http = c->data;
    state = http->state;
    //
    for (;;)
    {
        switch (state)
        {

            case proc_reqline:
                ret = ngd_http_proc_reqline(ngd_req_t *r);
                if (ret == NGD_OK) {
                    r.smethod.data = r.method_start;
                    r.smethod.len = r.method_end - r.method_start;
                    r.suri.data = r.uri_start;
                    r.suri.len = r.uri_end - r.uri_start;
                    r.sver.data = r.ver_start;
                    r.sver.len = r.ver_end - r.ver_start;
                    state = proc_headers;
                    break;
                }
                if (ret == NGD_AGAIN) {
                    ...
                }
                if (ret == NGD_ERR) {
                    ...
                }
                break;
            case proc_headers:
                ngd_http_proc_headers(ngd_http_t *http);
                if (ret == NGD_OK) {
                    r.suri.len =
                    state = proc_h
                }
                if (ret == NGD_AGAIN) {
                    ...
                }
                if (ret == NGD_ERR) {
                    ...
                }
                break;
            case proc_body:
                ngd_http_proc_body(ngd_http_t *http);
                break;
            case proc_build_resp:
                ngd_proc_build_resp(ngd_http_t *http);
                break;
            case proc_compress_resp:
                ngd_proc_compress_resp(ngd_http_t *http);
                break;
            case proc_send_resp:
                ngd_proc_send_resp(ngd_http_t *http);
                break;
        }
    }



    if (c->on_read) read();
    if (c->on_write) ..

    // if (c->on_timeout) ..
}


ngd_http_handle_read(ngd_http_t *http)
{
    enum {
        proc_reqline,
        proc_headers,
        proc_body,
        proc_build_response,
        proc_compress_response,
        proc_send_response
    } state;
    //
    state = http->state;
    //
    switch ()
    {
        case proc_reqline: break;
        case proc_headers: break;
        case proc_body: break;
    }
    http->state;
    //

}

ngd_http_handle_write(ngd_http_t *http)
{
    enum {
        proc_build_response,
        proc_compress_response,
        proc_send_response
    } state;
    //
    state = http->state;
    //
    switch (state)
    {
        case proc_build_response: break;
        case proc_compress_response: break;
        case proc_send_response:break;
        //
        case proc_ : break;

    }
    .. = state;

}

#endif
