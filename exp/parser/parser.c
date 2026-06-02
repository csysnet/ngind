#include "./parser.h"
#include "./structs.h"

/*BEGIN state handler*/
void
handle_state(conn *c)
{
    switch (c->state)
    {
        case PARSE_METHOD:
            if (req->method == NULL) req.url = pos;
            else if (buf[i] == ' '){
                buf[i] = '\0';
                state = PARSE_URL;
            }
            break;

        case PARSE_URL:
            if (req->url == NULL) req.url = pos;
            else if (buf[i] == ' ') {
                buf[i] = '\0'; state = PARSE_VER;
            }
            break;
        case PARSE_VER:
            if (req->ver == NULL) req.ver = pos;
            else if (buf[i] == '\r') {
                buf[i] = '\0';
            } else if (buf[i] == '\n' {
                state = PARSE_KEY;
            }
            break;


        case PARSE_KEY:
            if (req.headers[req.used_header].key == NULL) {
                req.headers[req.used_header].key = pos;
            } else if (buf[i] == ':')
                buf[i] = '\0';
            else if (buf[i] == ' ')
                state = PARSE_VALUE;
            break;

        case PARSE_VALUE:
            if (req.headers[req.used_header].key == NULL)
                req.headers[req.used_header].value = pos;
            } else if (buf[i] == '\r')
                buf[i] = '\0';
            else if (buf[i] == '\n')
                state = PARSE_VALUE_END;
                req.used_header;
            break;

        case PARSE_VALUE_END:
            if (buf[i] == '\n') {
                parse_header(conn_t *c);
                state = PARSE_BODY;

            }
            if (buf[i] != '\r') {
                req.headers[req.used_header].key = pos;
                state = PARSE_KEY;
            }
            break;
        case PARSE_BODY;


            break;

    }

}

void
parse_header(conn_t *c)
{
    for (int i=0; i<req.used_headers; i++)
    {

        if (strcasecmp(req->headers[i].key, "content-length") == 0)
        {
            req->content_len = strtoull(req->headers[i].value, NULL, 10)
        }
        if (strcasecmp(req->headers[i].key, "transfer-encoding"))
        {
            req->is_chunked = 1;
        }
        if (strcasecmp(req->headers[i].key, "connection") == 0)
        {
            if (strcasecmp(req->headers[i].value, "close") == 0)
                req->is_keep_alive = 0;
        }
    }
}
/*END state handler*/
