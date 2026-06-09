#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>
#include "./serv.h"


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

    u_char buf[BUFF_SIZE];
    handle_read(cli_fd, buf, BUFF_SIZE);


    send_ok(cli_fd, buf, sizeof(buf), "./index.html");
}
void
process_reqline(
    if ()
)
{

}
//per state, per answer before is
int
parse_reqline(ngd_req_t *r, ngd_buf_t *b)
{
    enum {
        ps_start=0,
        ps_method,
        ps_space_uri,
        ps_uri,
        ps_space_ver,
        ps_ver,
    } state;
    u_char *p;

    state = r->state;

    for (p = b->pos; p < b->last; p++) {
        switch (state)
        {
            case ps_start:
                (r->method).p = p;
                state = ps_method;
                break;
            case ps_method:
                if (*p == ' ') {
                    (r->method).len = p - (r->method).p;
                    state = ps_space_uri;
                }
                break;
            case ps_space_uri:
                (r->uri).p = p;
                state = ps_uri;
                break;
            case ps_uri:
                if (*p == ' ') {
                    (r->uri).len = p - (r->uri).p;
                    state = ps_space_ver;
                }
                break;
            case ps_space_ver:
                (r->ver).p = p;
                state = ps_ver;
                break;

            case ps_ver:
                if (*p == '\n') {
                    (r->ver).len = p - (r->ver).p - 1;
                    state = ps_start;
                }
                break;


        }

    }
        case PARSE_METHOD:
            // printf("reach  method");
            if (*p == ' ') {
                req->method.p = p - req->method.len;
                state = PARSE_URL;
                // printf("reach end method");
                // ps(req->method.p, req->method.len);
                continue;
            }
            req->method.len++;
            break;
        case PARSE_URL:
            if (*p == ' ') {
                req->url.p = p - req->url.len;
                state = PARSE_VER;
                // ps(req->url.p, req->url.len);
                continue;
            }
            req->url.len++;
            break;

        case PARSE_VER:
            if (*p == '\n') {
                req->ver.p = p - req->ver.len - 1;
                state = PARSE_HEADER;
                // ps(req->ver.p, req->ver.len);
                continue;
            }
            req->ver.len++;
            break;

        case PARSE_HEADER:
            if (*p == '\r') continue;
            else if (*p == '\n') state = PARSE_BODY;
            else {
                if (req->headers == NULL) {
                    req->headers = malloc(sizeof(ngd_hnode_t));
                } else {
                    ngd_hnode_t *last = req->headers;
                    if (last == NULL) {
                        req->headers  = malloc(sizeof(ngd_hnode_t))
                    }
                    while (last->next) last = last->next;
                    last->next = malloc(sizeof(ngd_hnode_t));
                    last->next->key.p = p;
                }
                state = PARSE_KEY;
            }
            break;

        // case PARSE_KEY:
        //     if (*p == ' ') {
        //         ngd_hnode_t *last = req->headers;
        //         while (last->next) last = last->next;
        //         //
        //         last->key.len = p - last->key.p - 1;
        //         continue;
        //     }
        //     break;
        // case PARSE_VALUE:
        //     if (*p == '\n') {
        //         ngd_hnode_t *last = req->headers;
        //         while (last->next) last = last->next;

        //         last->value.p = p - last->value.len;
        //         state = PARSE_HEADER;
        //     }
        //     ngd_hnode_t *last = req->headers;
        //     while (last->next) last = last->next;

        //     last->value.len++;
        //     break;

        // case PARSE_BODY:
        //     break;


    }
}


void
handle_read(int cli_fd, u_char *buf, size_t buflen)
{
    typedef enum {
        PARSE_METHOD,
        PARSE_URL,
        PARSE_VER,
        PARSE_KEY,
        PARSE_VALUE,
        PARSE_HEADER,
        PARSE_BODY
    } parse_state_t;

    ssize_t n_read;
    u_char *start, *pos, *last, *end;
    ngd_req_t *req;
    parse_state_t state;

    start = buf;
    pos = buf;
    last = buf;
    end = buf + buflen;

    req = malloc(sizeof(ngd_req_t));
    req->method.len = 0;
    req->url.len = 0;
    req->ver.len = 0;
    state = PARSE_METHOD;

    n_read = read(cli_fd, last, end - last);
    pos = last;
    last += n_read;

    for (u_char *p = pos; p != last; p++)
    {
        // printf("reach loop");
        switch (state)
        {
            case PARSE_METHOD:
                // printf("reach  method");
                if (*p == ' ') {
                    req->method.p = p - req->method.len;
                    state = PARSE_URL;
                    // printf("reach end method");
                    // ps(req->method.p, req->method.len);
                    continue;
                }
                req->method.len++;
                break;
            case PARSE_URL:
                if (*p == ' ') {
                    req->url.p = p - req->url.len;
                    state = PARSE_VER;
                    // ps(req->url.p, req->url.len);
                    continue;
                }
                req->url.len++;
                break;

            case PARSE_VER:
                if (*p == '\n') {
                    req->ver.p = p - req->ver.len - 1;
                    state = PARSE_HEADER;
                    // ps(req->ver.p, req->ver.len);
                    continue;
                }
                req->ver.len++;
                break;

            case PARSE_HEADER:
                if (*p == '\r') continue;
                else if (*p == '\n') state = PARSE_BODY;
                else {
                    if (req->headers == NULL) {
                        req->headers = malloc(sizeof(ngd_hnode_t));
                    } else {
                        ngd_hnode_t *last = req->headers;
                        if (last == NULL) {
                            req->headers  = malloc(sizeof(ngd_hnode_t))
                        }
                        while (last->next) last = last->next;
                        last->next = malloc(sizeof(ngd_hnode_t));
                        last->next->key.p = p;
                    }
                    state = PARSE_KEY;
                }
                break;

            // case PARSE_KEY:
            //     if (*p == ' ') {
            //         ngd_hnode_t *last = req->headers;
            //         while (last->next) last = last->next;
            //         //
            //         last->key.len = p - last->key.p - 1;
            //         continue;
            //     }
            //     break;
            // case PARSE_VALUE:
            //     if (*p == '\n') {
            //         ngd_hnode_t *last = req->headers;
            //         while (last->next) last = last->next;

            //         last->value.p = p - last->value.len;
            //         state = PARSE_HEADER;
            //     }
            //     ngd_hnode_t *last = req->headers;
            //     while (last->next) last = last->next;

            //     last->value.len++;
            //     break;

            // case PARSE_BODY:
            //     break;


        }
        // printf("reach end loop");
    }
    // ps(req->headers->key.p, req->headers->key.len);
    // ps(req->headers->value.p, req->headers->value.len);

    // for (ngd_hnode_t *cur = req->headers; cur != NULL; cur = cur->next) {
    // ps(cur->key.p, cur->key.len);
    // ps(cur->value.p, cur->value.len);
    //     printf("\n");
    // }

    ps(req->method.p, req->method.len);
    printf(" ");
    ps(req->url.p, req->url.len);
    printf(" ");
    ps(req->ver.p, req->ver.len);
    printf("\n");

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
