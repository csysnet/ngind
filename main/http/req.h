#ifndef REQ_H
#define REQ_H

#include "core.h"

#define MAX_INBUF 1024
#define HTTP_PARSE_HEADER_DONE 1

typedef struct str_t str_t;
typedef struct map_t map_t;
typedef struct conn_t conn_t;
typedef struct event_t event_t;
typedef struct buf_t buf_t;

typedef struct {
    str_t *smethod;
    str_t *suri;
    str_t *sver;
    long content_length;
    unsigned chunked;

    buf_t *header_in;
    map_t *headers;

    //cache
    int method;
    int state;
    //
    u_char *start_method;
    u_char *end_method;
    u_char *start_uri;
    u_char *end_uri;
    u_char *start_ver;
    u_char *end_ver;
    //
    u_char *start_key;
    u_char *end_key;
    u_char *start_value;
    u_char *end_value;

} req_t;

//
int http_wait_req(event_t *rev);
int http_empty_handler(event_t *wev);
int http_block_reading(event_t *rev);
int http_read_req(event_t *rev);
//
int http_proc_reqline(event_t *rev);
int http_proc_headers(event_t *rev);
int http_proc_body(event_t *rev);
int http_build_req(event_t *wev);
//
int http_conn_switch(event_t *rev);
//
int http_parse_reqline(req_t *r, buf_t *b);
int http_parse_header_line(req_t *r, buf_t *b);
#endif
