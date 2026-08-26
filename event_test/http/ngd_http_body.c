#include "ngd_http.h"
//
int
ngd_http_parse_body(ngd_http_t *http)
{
    enum {
        ps_start=NGD_STATE_START,
        ps_zero,
        ps_zero_cr,
        ps_zero_cr_lf,
        ps_zero_cr_lf_cr,
    } state;
    ngd_buf_t *b;
    u_char *p;
    int ret;
    ssize_t n;
    //
    state = http->state_parse;
    b = http->inbuf;
    //
    for (p = b->pos; p < b->last; p++)
    {
        switch (state)
        {
            case ps_start:
                if (*p == '0')
                    state = ps_
        }
    }
}
