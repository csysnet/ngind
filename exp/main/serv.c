#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

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
        ps(buf + total, buflen - total);
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

    char buf[BUFF_SIZE];
    // handle_read(cli_fd, buf, BUFF_SIZE);

    send_ok(cli_fd, buf, sizeof(buf), "./index.html");
}

void
handle_read(int cli_fd, u_char *buf, size_t buflen)
{
    ssize_t n_read;

    start = buf;

    pos = buf;
    last = buf;

    end = buf + buflen;


    total = 0;
    while (last == )
    {
        n_read = read(fd, last, end - last);
        pos = last;
        last += n_read;


        for (u_char p = pos; p != last; p++)
        {
            switch (state)
            {
                case PARSE_METHOD:
                    if (*p == ' ') {
                        state = PARSE_URL;
                        continue;
                    }
                    req->
                    break;


            }
        }
    }
    return total;
}

void
handle_state(cli_fd, buf, buflen, pos, last, state, req, p)
{
    switch (state)
    {
        case PARSE_METHOD:
            if (*p == ' '){
                req->method.p = p - req->method.len;
                state = PARSE_URL;
                continue;
            }
            req->method.len++;
            break;

        case PARSE_URL:
            if (*p == ' ') {
                req->url.p = p - req->method.len;
                state = PARSE_VER;
                continue;
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

            if (*p == '\n') {
                req->ver.p =
            }


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
