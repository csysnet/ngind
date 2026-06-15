#ifndef NGD_PSEUDO_H
#define NGD_PSEUDO_H
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#define NGD_OK 0
#define NGD_AGAIN -1



typedef struct {
    u_char *pos;
    u_char *last;
    off_t *fpos;
    off_t *flast;

    unsigned isfile;
    int fd;
    u_char *start;
    u_char *end;
} ngd_buf_t;

typedef struct {
    u_char  *p;
    size_t len;
} ngd_str_t;

typedef struct ngd_event_t {
    void *pdata;
    void (*handler)(struct ngd_event_t *ev);
} ngd_event_t;

typedef struct {
    ngd_str_t key;
    ngd_str_t value;
} hline;

static void
ps(void *buf, size_t buflen)
{
    for (int i = 0; i<buflen; i++)
        printf("%c", ((char*)buf)[i]);

}

typedef struct ngd_conn_t {
    int fd;
    ssize_t (*recv)(ngd_conn_t *c,
                       u_char *buf,
                       size_t size);



    ngd_event_t *read;
    ngd_event_t *write;
    ngd_buf_t *buf;
    void *pdata;
} ngd_conn_t;


ssize_t
ngd_unix_recv(ngd_conn_t *c,
              u_char *buf,
              size_t size)
{
    return recv(c->fd, buf, size, 0);
}
#endif
