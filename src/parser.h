#ifndef PARSER_H
#define PARSER_H

//AFTER
enum State {
    STATE_BEGIN

    STATE_METHOD,
    STATE_METHOD_SPACE,
    STATE_URL,
    STATE_URL_SPACE,
    STATE_VER,
    STATE_VER_CR,
    STATE_VER_CR_LR,

    STATE_KEY,
    STATE_KEY_COLON
    STATE_VALUE,
    STATE_VALUE_CR,
    STATE_VALUE_CR_LF,
    STATE_VALUE_CR_LF_CR,
    STATE_VALUE_CR_LF_CR_LF,

    STATE_BODY
}

void
shit(int fd, void *buf, size_t buflen)
{
    ssize_t n_read = read(fd, buf, buflen)



        switch (state)
        {
            case STATE_BEGIN:
                req.line.method.s = buf; state = STATE_METHOD;
            case STATE_METHOD:
                if (buf[i] == ' ') state = STATE_METHOD_SPACE;
                else req.line.method.slen++;
            case STATE_METHOD_SPACE:
                req.line.url.s = &buf[i]; state = STATE_URL;
            case STATE_URL:
                if (buf[i] == ' ') state = STATE_URL_SPACE;
                else req.line.url.slen++;

            case STATE_URL_SPACE:
                state = STATE_VER; req.line.ver.s = &buf[i];
            case STATE_VER:
                if (buf[i] == '\r') state = STATE_VER_CR;
                else req.line.ver.slen++;


            case STATE_VER_CR:
                if (buf[i] == '\n') state = STATE_VER_CR_LR;

            case STATE_VER_CR_LR:
                state = STATE_KEY; req.header.key.s = &buf[i];


            case STATE_KEY:
                if (buf[i] == ':') state = STATE_KEY_COLON
                else if (buf[i] == ' ') continue;
                else req.line.;
            case STATE_KEY_COLON:
                if (buf[i] != ' ') state = STATE_VALUE; req.header.value.s = &buf[i];
            case STATE_VALUE:
                if (buf[i] == '\r') state = STATE_VALUE_CR;
                else ...apened;

            case STATE_VALUE_CR:
                if (buf[i] == '\n') state = STATE_VALUE_CR_LF;

            case STATE_VALUE_CR_LF:
                if (buf[i] == '\r') state = STATE_VALUE_CR_LF_CR;
                else {
                    state = STATE_KEY; LOOOP
                    new header and its slen
                }
            case STATE_VALUE_CR_LF_CR:
                if (buf[i] == '\n') state = STATE_VALUE_CR_LF_CR_LF;

            case STATE_VALUE_CR_LF_CR_LF:
                state = STATE_BODY; req.body.payload.s = &buf[i]

            case STATE_BODY:
                if (i < conlen) req.body.payload.slen++;
                else STATE_END
            case STATE_END:
        }



}


#endif
