#ifndef STRUCTS_H
#define STRUCTS_H

/*BEGIN directives*/
#define BUFLEN 8192
/*END directives*/

/*BEGIN state*/
typedef enum {
    PROC_PARSE;
    PROC_RES;
    PROC_SEND;
    PROC_CLOSE;
} proc_state_t;

typedef enum {
    PARSE_METHOD,
    PARSE_URL,
    PARSE_VER,
    PARSE_VER_CR,

    PARSE_KEY,
    PARSE_VALUE,
    PARSE_VALUE_CR,
    PARSE_VALUE_CR_LF
    PARSE_VALUE_CR_LF_CR,

    PARSE_BODY,
    PARSE_DONE,
} parse_state_t;


/*END state*/

/*BEGIN request*/

typedef struct {
    char *key;
    char *value;
    char *next;
} header_t;

typedef struct {
    //request line
    char *method;
    char *url;
    char *ver;

    //headers
    header_t *ll_header;
    //cache headers
    size_t content_length;
    //
    char is_keep_alive;
    char is_chunked;
    char is_content_length;

    parse_state_t parse_state;
    //body
    void* payload;
    size_t paylen;
} req_t;

typedef struct {
    int temp_fd;
    size_t receive;
    size_t expect;
} req_body_t
/*END request*/

higher copy tempbuf to this buf

/*BEGIN client*/
typedef struct {
    int sfd;
    state_t state;

    //buf
    char buf[8192];
    size_t byte_recv;

    //pos
    size_t w
    size_t r_pos

    //req
    req_t req;
} cli_t;
/*END client*/

#endif
