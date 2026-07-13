#include "req.h"


int
http_parse_reqline(req_t *r, buf_t *b)
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
    char c;
    //
    state = r->state;
    //
    for (p = b->pos; p < b->last; p++)
    {
        c = *p;
        switch (state)
        {
            case ps_start:
                r->start_method = p;
                state = ps_method;
            case ps_method:
                if (c == ' ') {
                    r->end_method = p; state = ps_space_uri;
                    break;
                }
                break;
            case ps_space_uri:
                r->start_uri = p;
                state = ps_uri;
                break;
            case ps_uri:
                if (c == ' ') {
                    r->end_uri = p; state = ps_space_ver;
                    break;
                }
                break;
            case ps_space_ver:
                r->start_ver = p;
                state = ps_ver;
                break;

            case ps_ver:
                if (c == '\n') {
                    r->end_ver = p - 1;
                    goto done:
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

int
http_parse_header_line(req_t *r, buf_t *b)
{
    enum {
        ps_start=0,
        ps_key,
        ps_space_value,
        ps_value
    } state;
    u_char *p;
    char c;
    //
    state = r->state;
    //
    for (p = b->pos; p < b->last; p++)
    {
        c = *p;
        switch (state)
        {
            case ps_start:
                if (*p == '\r')
                    break;

                if (*p == '\n')
                    goto header_done;

                r->start_key = p;
                state = ps_key;
                break;
            case ps_key:
                if (c == ' ') {
                    r->end_key = p - 1;
                    state = ps_space_value;
                    break;
                }
                break;
            case ps_space_value:
                r->start_value = p;
                state = ps_value;
                break;

            case ps_value:
                if (c == '\n') {
                    r->end_value = p - 1;
                    goto done;
                }
                break;
        }
    }
    //
    b->pos = b->last;
    r->state = state;
    return NGD_AGAIN;

done:
    b->pos = p + 1;
    r->state = ps_start;
    return NGD_OK;
header_done:
    b->pos = p + 1;
    r->state = ps_state;
    return HTTP_PARSE_HEADER_DONE;
}
