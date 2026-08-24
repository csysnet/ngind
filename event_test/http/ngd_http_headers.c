#include "ngd_http.h"
//
int
ngd_http_handle_headers(ngd_http_t *http)
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
    state = http->state;
    b = http->inbuf;
    //
    for (;;)
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
    http->state = state;
    return NGD_AGAIN;
done:
    b->pos = p + 1;
    http->state = ps_start;
    return NGD_OK;
header_done:
    b->pos = p + 1;
    http->state = ps_start;
    return NGD_HTTP_FULL_HEADER_DONE;
}
