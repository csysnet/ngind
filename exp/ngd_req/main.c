#include "ngd_req.h"

// typedef struct {
//     int epfd;
//     int max_events;
//     struct epoll_event *events;

//     // timer_manager_t timers;
//     // queue_t posted_events
// } ngd_event_cxt_t;
void
test()
{
    ngd_req_t r;
    ngd_buf_t b;

    int pstate;

    u_char buf[8192];
    int n;

    n = snprintf((char *)buf, 8192,
        "GET /dat HTTP/1.1\r\n"
        "Host: dat.com\r\n"
        "Content-length: 800\r\n"
        // "Connection: keep-alive\r\n"
        "\r\n"
    );


    b.start = buf;
    b.pos = b.start;
    b.last = b.start;
    b.end = b.start + 8192;

    b.last += n;

    r.state = 0;
    ngd_parse_reqline(&r, &b);
    printf("start: %lu\n", r.start_method);
    printf("end: %lu\n", r.end_method);
    ps(r.start_method, r.end_method - r.start_method);
    printf(" ");
    ps(r.start_uri, r.end_uri - r.start_uri);
    printf(" ");
    ps(r.start_ver, r.end_ver - r.start_ver);
    printf("\n");


    ngd_parse_header_line(&r, &b);

    ps(r.start_key, r.end_key - r.start_key);
    printf(":");
    ps(r.start_value, r.end_value-r.start_value);
    printf("\n");

    ngd_parse_header_line(&r, &b);

    ps(r.start_key, r.end_key - r.start_key);
    printf(":");
    ps(r.start_value, r.end_value-r.start_value);
    printf("\n");

    pstate = ngd_parse_header_line(&r, &b);
    if (pstate == NGD_PARSE_HEADER_DONE) {
        printf("ending\n");
    } else {
        ps(r.start_key, r.end_key - r.start_key);
        printf(":");
        ps(r.start_value, r.end_value-r.start_value);
        printf("\n");
    }
}

int
main()
{
    test();
}
