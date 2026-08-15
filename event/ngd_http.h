#ifndef NGD_HTTP_CONN_H
#define NGD_HTTP_CONN_H
//
typedef struct ngd_http_t ngd_http_t;
typedef struct ngd_req_t ngd_req_t;
//
struct ngd_http_t {
    ngd_req_t *r;
    int state;
    //
    ngd_buf_t *in;
    ngd_buf_t *out;
    //
};

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

ngd_http_handle(ngd_conn_t *c)
{
    ngd_http_t *http = c->data;

    if (c->on_timeout) ..
    if (c->on_read) ..
    if (c->on_write) ..

}



ngd_http_handle_read(ngd_http_t *http)
{
    enum {
        proc_reqline,
        proc_headers,
        proc_body,
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
