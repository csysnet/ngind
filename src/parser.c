#include "./parser.h"
#include "./structs.h"


/*BEGIN state sub-handler*/
void
handle_method(cli_t *cli)
{
    if (cli->method)
        cli->state = STATE_METHOD
        cli->req.method.p = cli->w_buf + r_pos;
        cli->req.method.len = 1;

    if (buf[i] == ' ') {
        req.line.url.s = &buf[i]; state = STATE_URL;
    }
    else req.line.method.slen++;
}
void
handle_url(cli_t *cli)
{
    if (buf[i] == ' ') {
        state = STATE_VER; req.line.ver.s = &buf[i];

    }
    else req.line.url.slen++;
}

void
handle_ver(cli_t *cli)
{
    if (buf[i] == '\r') state = STATE_VER_CR;
    else req.line.ver.slen++;
}

void
handle_ver_cr(cli_t *cli)
{
    if (buf[i] == '\n') {
        state = STATE_KEY; req.header.key.s = &buf[i];
    }
}

void
handle_key(cli_t *cli)
{
    if (buf[i] == ':') state = STATE_VALUE; req.header.value.s = &buf[i];
    else req.line.;
}

void
handle_value(cli *cli)
{
    if (buf[i] == '\r') state = STATE_VALUE_CR;
    else ...apened;
}

void
handle_value_cr(cli_t *cli)
{
    if (buf[i] == '\n'){
        state = STATE_VALUE_CR_LF;
    }
    else append;
}

void
handle_value_cr_lf(cli_t *cli)
{
    if (buf[i] == '\r') state = STATE_VALUE_CR_LF_CR;
    else {
        state = STATE_KEY; LOOOP
        new header and its slen
    }
}

void
handle_cr_lf_cr(cli_t *cli)
{
    if (buf[i] == '\n') {
        state = STATE_BODY; req.body.payload.s = &buf[i]
    }
}

void
handle_body(cli_t *cli)
{
    if (i < conlen) req.body.payload.slen++;
    else STATE_END
}
/*END state sub-handler*/


/*BEGIN state handler*/
void
handle_state(cli_t *cli)
{
    switch (state)
    {
        case STATE_METHOD: handle_method(cli); break;
        case STATE_URL: handle_url(cli); break;
        case STATE_VER: handle_ver(cli); break;
        case STATE_VER_CR: handle_ver_cr(cli); break;
        case STATE_KEY: handle_key(cli); break;
        case STATE_VALUE: handle_value(cli); break;
        case STATE_VALUE_CR: handle_value_cr(cli); break;
        case STATE_VALUE_CR_LF: handle_value_cr_lf(cli); break;
        case STATE_VALUE_CR_LF_CR: handle_value_cr_lf_cr(cli); break;
        case STATE_BODY: handle_body(cli); break;
    }

}
/*END state handler*/
