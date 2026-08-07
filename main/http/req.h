#ifndef REQ_H
#define REQ_H

#include "core.h"

#define MAX_INBUF 100000
#define HTTP_MAX_BODY_MEM 1024
#define HTTP_MAX_BODY (1024 * 1024 * 1024 * 1024)
#define HTTP_PARSE_HEADER_DONE 1
#define HTTP_CHUNKING_DONE 1

typedef struct str_t str_t;
typedef struct map_t map_t;
typedef struct conn_t conn_t;
typedef struct event_t event_t;
typedef struct buf_t buf_t;

typedef struct {
    //line
    str_t *smethod;
    str_t *suri;
    str_t *sver;
    //headers
    buf_t *header_in;
    map_t *headers;
    long content_length;
    //body
    unsigned chunked;
    size_t body_received;
    size_t chunk_size;
    u_char *start_chunk;
    u_char *end_chunk;
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
    //
    unsigned proxy:1;
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
int http_proc_body_on_file(event_t *rev);
int http_build_req(event_t *wev);
//
int http_proc_switch(event_t *rev);
//
int http_parse_reqline(req_t *r, buf_t *b);
int http_parse_header_line(req_t *r, buf_t *b);
int http_parse_body(req_t *r, buf_t *b);
#endif
