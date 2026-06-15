#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>
#include "./serv.h"



void ngd_proc_reqline(ngd_event_t *ev);
int ngd_parse_reqline(ngd_req_t *r, ngd_buf_t *b);

void ngd_proc_headers(ngd_event_t *ev);
int ngd_parse_header_line(ngd_req_t *r, ngd_buf_t *b);

void ngd_proc_body(ngd_event_t *ev);
int ngd_parse_body(ngd_event_t *ev, ngd_buf_t *b);


int
ngd_parse_reqline(ngd_req_t *r, ngd_buf_t *b)
{
    enum {
        ps_start=0,
        ps_method,
        ps_uri,
        ps_ver,
        ps_end
    } state;

    u_char *pos, *last;

    pos = b->pos;
    last = b->last;

    state = r->state;

    for (u_char *p = pos; p < last; p++)
    {
        //today,tomorrow
        switch (state)
        {
            case ps_start:
                r->begin_req = p;
                r->end_method = NULL;
                r->begin_uri = NULL;
                r->end_uri = NULL;
                r->begin_ver = NULL;
                r->end_ver = NULL;
                state = ps_method;
                break;
            case ps_method:
                if (*p == ' ') {
                    r->end_method = p;
                    state = ps_uri;
                    break;
                }
                break;
            case ps_uri:
                if (r->begin_uri == NULL){
                    r->begin_uri = p;
                    break;
                }

                if (*p == ' ') {
                    r->end_uri = p;
                    state = ps_ver;
                    break;
                }
                break;

            case ps_ver:

                if (r->begin_ver == NULL) {
                    r->begin_ver = p;
                    break;
                }

                if (*p == '\n') {
                    r->end_ver = p - 1;
                    state = ps_end;
                    break;
                }
                break;
            case ps_end:
                b->pos = p;
                return NGD_OK;
        }
    }
    b->pos = last;
    return NGD_AGAIN;
}

int
ngd_parse_header_line(ngd_req_t *r, ngd_buf_t *b)
{
    enum {
        ps_start=0,
        ps_key,
        ps_value,
        ps_header,
        ps_end,
        ps_body,
    } state;
    u_char *pos, *last;


    pos = b->pos;
    last = b->last;

    state = r->state;

    for (u_char *p = pos; p < last; p++)
    {
        //today,tomorrow
        switch (state)
        {
            case ps_start:
                r->begin_key = p;
                r->end_key = NULL;
                r->begin_value = NULL;
                r->end_value = NULL;
                state = ps_header;
                break;
            case ps_header:
                if (*p == '\r') {
                    break;
                } else if (*p == '\n') {
                    state = ps_body;
                    break;
                }
            case ps_key:
                if (r->begin_key == NULL) {
                    r->begin_key = p;
                    break;
                }
                if (*p == ' ') {
                    r->end_key = p - 1;
                    state = ps_value;

                    break;
                }
                break;
            case ps_value:
                if (r->begin_value == NULL) {
                    r->begin_value = p;
                    break;
                }

                if (*p == '\n') {
                    r->end_value = p - 1;
                    state = ps_end;
                    break;
                }
                break;
            case ps_body:
                return 1;
            case ps_end:
                return 1;
        }
    }
    return 1;
}









//if current isn't enough, try to move unparsed part to new one, not the whole because the past still there
off_t
get_flen(int fd)
{
    struct stat st;

    fstat(fd, &st);

    return st.st_size;
}

void
send_msg(int fd, void *buf, size_t buflen)
{
    int nwrite, total;

    total = 0;
    while (total < buflen)
    {
        nwrite = write(fd, buf + total, buflen - total);
        // ps(buf + total, buflen - total);
        total += nwrite;
    }
}

void
ps(void *buf, size_t buflen)
{
    for (int i = 0; i<buflen; i++)
        printf("%c", ((char*)buf)[i]);

}

void
send_ok(int cli_fd, void *buf, size_t buflen, const char *fname)
{
    int fd;
    ssize_t nread, nwrite;
    size_t twrite;
    off_t flen;

    fd = open(fname, O_RDONLY);
    flen = get_flen(fd);

    snprintf(buf, buflen, RES_OK, flen);

    send_msg(cli_fd, buf, strlen(buf));
    while (1)
    {
        nread = read(fd, buf, BUFF_SIZE);
        if (nread==0) break;
        send_msg(cli_fd, buf, nread);
    }
}

void
handle_cli(int cli_fd)
{
    ngd_buf_t buf;
    buf.start = malloc(BUFF_SIZE);
    buf.end = buf.start + BUFF_SIZE;
    buf.pos = buf.start;
    buf.last = buf.start;
    handle_read(cli_fd, &buf);

    send_ok(cli_fd, buf.start, BUFF_SIZE, "./index.html");
    printf("send_ok\n");
}
//per state, per

//this layer only know that parsing through pointer
// han che xu dung continue
//
//read until pos==last, return back, that its!!
void
ngd_lex_reqline(ngd_req_t *r, ngd_buf_t *buf)
{
    enum {
        PARSE_BEGIN=0,
        PARSE_METHOD,
        PARSE_URL,
        PARSE_VER,
        PARSE_KEY,
        PARSE_VALUE,
        // PARSE_VALUE_CR,
        PARSE_HEADER,
        PARSE_BODY,
        // PARSE_END,
    } state;
    u_char *pos, *last;

    state = r->state;

    pos = buf->pos;
    last = buf->last;
    for (u_char *p = pos; p < last; p++)
    {
        //today,tomorrow
        switch (state)
        {
            case PARSE_BEGIN:
                r->begin_req = p;
                r->end_method = NULL;
                r->begin_uri = NULL;
                r->end_uri = NULL;
                r->begin_ver = NULL;
                r->end_ver = NULL;
                r->begin_key = NULL;
                r->end_key = NULL;
                r->begin_value = NULL;
                r->end_value = NULL;
                state = PARSE_METHOD;
                break;
            case PARSE_METHOD:
                if (*p == ' ') {
                    r->end_method = p;
                    state = PARSE_URL;
                    break;
                }
                break;
            case PARSE_URL:
                if (r->begin_uri == NULL){
                    r->begin_uri = p;
                    break;
                }

                if (*p == ' ') {
                    r->end_uri = p;
                    state = PARSE_VER;
                    break;
                }
                break;

            case PARSE_VER:

                if (r->begin_ver == NULL) {
                    r->begin_ver = p;
                    break;
                }

                if (*p == '\n') {
                    r->end_ver = p - 1;
                    state = PARSE_KEY;
                    break;
                }
                break;

            case PARSE_KEY:
                if (r->begin_key == NULL) {
                    r->begin_key = p;
                    break;
                }
                if (*p == ' ') {
                    r->end_key = p - 1;
                    state = PARSE_VALUE;

                    break;
                }
                break;
            case PARSE_VALUE:
                if (r->begin_value == NULL) {
                    r->begin_value = p;
                    break;
                }

                if (*p == '\n') {
                    r->end_value = p - 1;
                    state = PARSE_HEADER;
                    break;
                }
                break;
            case PARSE_HEADER:
                if (*p == '\r') {
                    break;
                } else if (*p == '\n') {
                    state = PARSE_BODY;
                    break;
                } else {
                    r->headers[r->hpos].key.p = r->begin_key;
                    r->headers[r->hpos].key.len = r->end_key - r->begin_key;
                    r->headers[r->hpos].value.p = r->begin_value;
                    r->headers[r->hpos].value.len = r->end_value - r->begin_value;


                    r->begin_key = p;
                    r->begin_value = NULL;
                    r->hpos++;
                    state = PARSE_KEY;
                    break;
                }
            case PARSE_BODY:
                return;
                break;
        }
    }
    return;
}

void
handle_read(int cli_fd, ngd_buf_t *buf)
{

    ssize_t n_read;
    ngd_req_t *r;

    n_read = read(cli_fd, buf->last, buf->end - buf->last);
    buf->pos = buf->last;
    buf->last += n_read;
    r->state = 0;
    r->hpos = 0;
    ngd_lex_reqline(r, buf);

    printf("start hcount: %lu\n", r->hpos);
    ps(r->begin_req, r->end_method - r->begin_req);
    printf(" ");
    ps(r->begin_uri, r->end_uri - r->begin_uri);
    printf(" ");
    ps(r->begin_ver, r->end_ver - r->begin_ver);
    printf("\n");
    for (int i=0; i<r->hpos; i++) {
        ps(r->headers[i].key.p, r->headers[i].key.len);
        printf(":");
        ps(r->headers[i].value.p, r->headers[i].value.len);
        printf("\n");
    }
    printf("end hcount: %lu\n", r->hpos);

}

int main()
{
    start();
}


void start()
{
    int serv_fd, cli_fd;
    int n_read;
    struct sockaddr_in addr;

    serv_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(serv_fd, (struct sockaddr *)&addr, sizeof(addr));

    listen(serv_fd, BACKLOG);

    while (1) {
        cli_fd = accept(serv_fd, (struct sockaddr *)NULL, NULL);

        handle_cli(cli_fd);
        close(cli_fd);
    }

}
