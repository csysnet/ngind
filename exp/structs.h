#ifndef STRUCTS_H
#define STRUCTS_H
//CHI DUOC 1 DAU CHUYEN TRANG THAI HOAC LA DAU
/*BEGIN directives*/
#define BUFLEN 8192
/*END directives*/

/*BEGIN state*/
enum {
    PROC_PARSE;
    PROC_RES;
    PROC_SEND;
    PROC_CLOSE;
} proc_state_t;

typedef enum {
    PARSE_METHOD,
    PARSE_URL,
    PARSE_VER,

    PARSE_KEY,
    PARSE_VALUE,
    PARSE_VALUE_END,

    PARSE_BODY,
    PARSE_DONE,
} parse_state_t;


/*END state*/

/*BEGIN request*/
#define HEADER_COUNT 64
typedef struct {
    char *key;
    char *value;
} header_t;


typedef struct {
    //request line
    char *method;
    char *url;
    char *ver;

    //headers
    header_t headers[HEADER_COUNT];
    size_t used_header;
    //cache headers
    size_t content_len;
    //
    char is_keep_alive;
    char is_chunked;

    parse_state_t parse_state;
    //body
    req_body_t body;
} req_t;

typedef struct {
    int tempfd;
    void* payload;
    size_t paylen;
} req_body_t
/*END request*/

higher copy tempbuf to this buf

/*BEGIN client*/


typedef struct {
    proc_state_t proc_state;
    parse_state_t parse_state;

    //buf
    char inbuf[16384];
    size_t pos;
    size_t last;



    //req
    req_t req;
} http_ctx_t;
/*END client*/

typedef struct {
    u_char  *p
    size_t len;
} ngd_str_t;

typedef struct ngd_hnode {
    ngd_str_t key;
    ngd_str_t value;
    //
    struct ngd_hnode *next;
} ngd_hnode_t;

typedef struct {
    //req line
    ngd_str_t method;
    ngd_str_t url;
    ngd_str_t ver;
    //hlines
    ndg_hnode *headers;
    //body
    ndg_req_body_t body;
} ngd_req_t;

typedef struct {

} ngd_req_body_t;

//header_in for header
//body have tempbuf, buf chain, buf file
typedef struct {
    enum {
        BUF_MEM
        BUF_FILE
    } type;
    //mem
    u_char *start;
    u_char *pos;
    u_char *last;
    u_char *end;
    //file
    ngd_temp_file *file;
    off_t file_pos;
    off_t file_last;
} ngd_buf_t;
//multiple buf to one temp file
//chain save a subset of temp file
// one recv, one process, unproces up front
typedef struct {
    ngx_file_t file;

    ngx_path_t *path;

    off_t offset;

    unsigned persistent:1;
    unsigned clean:1;
} ngx_temp_file_t;

#endif
