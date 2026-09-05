//
#include <stddef.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>


#define NGD_OK 0
#define NGD_ERR -1
#define NGD_AGAIN -2

typedef unsigned char u_char;

typedef struct buf_t {
    u_char *start;
    u_char *pos;
    u_char *last;
    u_char *end;
    //
    int fd;
    off_t fpos;
    off_t flast;
    unsigned file:1;
} buf_t;

typedef struct {
    int state;
} req_t;
//
// typedef struct {
//     int epfd;
//     int max_events;
//     struct epoll_event *events;

//     // timer_manager_t timers;
//     // queue_t posted_events
// } ngd_event_cxt_t;
void
ps(u_char *p, size_t len)
{

    // printf("keylen: %lu\n", s->len);
    for (int i = 0; i<len; i++)
        printf("%c", p[i]);
}
// void
// test()
// {
//     ngd_req_t r;
//     ngd_buf_t b;

//     int pstate;

//     u_char buf[8192];
//     int n;

//     n = snprintf((char *)buf, 8192,
//         "GET /dat HTTP/1.1\r\n"
//         "Host: dat.com\r\n"
//         "Content-length: 800\r\n"
//         // "Connection: keep-alive\r\n"
//         "\r\n"
//     );


//     b.start = buf;
//     b.pos = b.start;
//     b.last = b.start;
//     b.end = b.start + 8192;

//     b.last += n;

//     r.state = 0;
//     ngd_parse_reqline(&r, &b);
//     printf("start: %lu\n", r.start_method);
//     printf("end: %lu\n", r.end_method);
//     ps(r.start_method, r.end_method - r.start_method);
//     printf(" ");
//     ps(r.start_uri, r.end_uri - r.start_uri);
//     printf(" ");
//     ps(r.start_ver, r.end_ver - r.start_ver);
//     printf("\n");


//     ngd_parse_header_line(&r, &b);

//     ps(r.start_key, r.end_key - r.start_key);
//     printf(":");
//     ps(r.start_value, r.end_value-r.start_value);
//     printf("\n");

//     ngd_parse_header_line(&r, &b);

//     ps(r.start_key, r.end_key - r.start_key);
//     printf(":");
//     ps(r.start_value, r.end_value-r.start_value);
//     printf("\n");

//     pstate = ngd_parse_header_line(&r, &b);
//     if (pstate == NGD_PARSE_HEADER_DONE) {
//         printf("ending\n");
//     } else {
//         ps(r.start_key, r.end_key - r.start_key);
//         printf(":");
//         ps(r.start_value, r.end_value-r.start_value);
//         printf("\n");
//     }
// }
#define PSEUDO_BODY "0\r\n" \
                    "\r\n" \
                    "phamtandat" \
                    "\n"
int
http_parse_body(req_t *r, buf_t *b)
{
    // 3\r\ndat\r\n0\r\n\r\n
    enum {
        ps_start=0,
        ps_0,
        ps_0_r,
        ps_0_r_n,
        ps_0_r_n_r,
    } state;
    u_char *p;
    u_char c;
    //
    state = r->state;
    //
    for (p = b->pos; p < b->last; p++)
    {
        c = *p;
        switch (state)
        {
            case ps_start:
                if (c == '0')
                    state = ps_0;
                break;
            case ps_0:
                if (c == '\r')
                    state = ps_0_r;
                else
                    state = ps_start;
                break;
            case ps_0_r:
                if (c == '\n')
                    state = ps_0_r_n;
                else
                    state = ps_start;
                break;
            case ps_0_r_n:
                if (c == '\r')
                    state = ps_0_r_n_r;
                else
                    state = ps_start;
                break;
            case ps_0_r_n_r:
                if (c == '\n')
                    goto done;
                else
                    state = ps_start;
                break;
        }
    }
    b->pos = b->last;
    r->state = state;
    return NGD_AGAIN;
done:
    b->pos = p + 1;
    r->state = ps_start;

    printf("len: %lu\n", sizeof (PSEUDO_BODY));
    ps(b->start, sizeof (PSEUDO_BODY) - 2);
    return NGD_OK;
}
void
test1()
{
    req_t r;
    buf_t b;
    int pstate;

    u_char buf[8192];
    int n;

    n = snprintf((char *)buf, 8192,
        PSEUDO_BODY
    );


    b.start = buf;
    b.pos = b.start;
    b.last = b.start;
    b.end = b.start + 8192;

    b.last += n;

    r.state = 0;
    http_parse_body(&r, &b);
}

int
main()
{
    test1();
}
