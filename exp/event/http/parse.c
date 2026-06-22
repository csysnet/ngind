#include "http.h"

int
ngd_http_parse_reqline(ngd_req_t *r, ngd_buf_t *b)
{
    enum {
        ps_start=0,
        ps_method,
        ps_space_uri,
        ps_uri,
        ps_space_ver,
        ps_ver
    } state;

    u_char *p;
    state = r->state;

    for (p = b->pos; p < b->last; p++)
    {
        switch (state)
        {
            case ps_start:
                r->start_method = p;
                state = ps_method;
            case ps_method:
                if (*p == ' ') {
                    r->end_method = p; state = ps_space_uri;
                    break;
                }
                break;
            case ps_space_uri:
                r->start_uri = p;
                state = ps_uri;
                break;
            case ps_uri:
                if (*p == ' ') {
                    r->end_uri = p; state = ps_space_ver;
                    break;
                }
                break;
            case ps_space_ver:
                r->start_ver = p;
                state = ps_ver;
                break;
            case ps_ver:
                if (*p == '\n') {
                    r->end_ver = p - 1;
                    goto done;
                }
                break;
        }
    }

    b->pos = b->last;
    r->state = state;
    return NGD_AGAIN;

done:
    b->pos = p + 1;
    r->state = ps_start;
    return NGD_OK;
}
