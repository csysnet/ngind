#ifndef NGD_HTTP_CONN_H
#define NGD_HTTP_CONN_H
//
typedef struct ngd_http_t ngd_http_t;
//
struct ngd_http_t {
    ngd_req_t *r;
    int state;
    //
    ngd_buf_t *in;
    ngd_buf_t *out;
    //
};

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
        proc_body
    } state;
    switch (http->state)
    {

    }
}

ngd_http_handle_write(ngd_http_t *http)
{
    enum {
        proc_build_response,
        proc_compress_response,
        proc_send_response
    } state;
    //
    state = ..;

    switch (state)
    {
        ...
    }
    .. = state;

}
#endif
