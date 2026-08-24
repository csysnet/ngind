#include "ngd_http.h"
//
int
ngd_http_handle_reqline(ngd_http_t *http)
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
    int ret;
    ssize_t n;
    //
    state = http->state;
    b = http->inbuf;
    //
    for (p = b->pos; p < b->last; p++)
    {
        switch (state)
        {
            case ps_start:
                http->method_start = p;
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
                if (c == '\n') {
                    http->ver_end = p - 1;
                    goto done;
                }
                break;
        }
    }
    b->pos = b->last;
    http->state = state;
    return NGD_AGAIN;
done:
    b->pos = p + 1;
    http->state = ps_start;
    return NGD_OK;
}
