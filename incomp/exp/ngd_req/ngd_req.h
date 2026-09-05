#ifndef NGD_REQ_H
#define NGD_REQ_H

typedef struct {
    int epfd;
    int max_events;
    struct epoll_event *events;

    // timer_manager_t timers;
    // queue_t posted_events
} ngd_event_cxt_t;

#include "ngd_pseudo.h"
#define NGD_PARSE_HEADER_DONE 1

typedef struct {
    //req line
    ngd_str_t smethod;
    ngd_str_t suri;
    ngd_str_t sver;

    ngd_buf_t *header_in;


    hline headers[64];
    size_t hpos;

    //owned by parser
    //request line
    int state;
    int method;
    //
    //
    int method;
    u_char *start_method;
    u_char *end_method;
    u_char *start_uri;
    u_char *end_uri;
    u_char *start_ver;
    u_char *end_ver;
    //header
    u_char *start_key;
    u_char *end_key;
    u_char *start_value;
    u_char *end_value;


    // //body
    // ndg_req_body_t body;

} ngd_req_t;



//util
int ngd_read_req(ngd_event_t *rev);
int ngd_wait_req(ngd_event_t *rev);

//proc

int ngd_proc_reqline(ngd_event_t *rev);
int ngd_proc_headers(ngd_event_t *rev);
int ngd_proc_body(ngd_event_t *rev);

//parse
int ngd_parse_reqline(ngd_req_t *r, ngd_buf_t *b);
int ngd_parse_header_line(ngd_req_t *r, ngd_buf_t *b);
#endif
