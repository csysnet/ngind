#ifndef NGD_REQ_H
#define NGD_REQ_H

#include <sys/types.h>

typedef struct ngd_str_t ngd_str_t;
typedef struct ngd_buf_t ngd_buf_t;
typedef struct ngd_conn_t ngd_conn_t;
typedef struct ngd_event_t ngd_event_t;

#define NGD_PARSE_HEADER_DONE 1

typedef struct {
    ngd_str_t *key;
    ngd_str_t *value;
} hline;

typedef struct {
    //reqline
    ngd_str_t *smethod;
    ngd_str_t *suri;
    ngd_str_t *sver;

    ngd_buf_t *header_in;

    hline headers[128];
    size_t hpos;

    //cache
    int state;
    int method;
    //
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
} ngd_req_t;

//
int ngd_http_init_conn(ngd_conn_t *c);
int ngd_wait_req(ngd_event_t *rev);
int ngd_http_empty_handler(ngd_event_t *wev);
int ngd_http_block_reading(ngd_event_t *rev);
int ngd_http_conn_switch(ngd_event_t *rev);
int ngd_http_read_req(ngd_event_t *rev);
//
int ngd_http_proc_reqline(ngd_event_t *rev);
int ngd_http_proc_headers(ngd_event_t *rev);
int ngd_http_proc_body(ngd_event_t *rev);
int ngd_http_build_res(ngd_event_t *wev);
//
int ngd_http_parse_reqline(ngd_req_t *r, ngd_buf_t *b);
int ngd_http_parse_header_line(ngd_req_t *r, ngd_buf_t *b);
#endif
