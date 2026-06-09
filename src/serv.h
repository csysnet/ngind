#ifndef SERV_H
#define SERV_H

#define PORT 8081
// #define PORT 8081
// #define PORT 8082
#define BACKLOG 10
#define BUFF_SIZE 4096

typedef unsigned char u_char;

typedef struct {
    u_char  *p;
    size_t len;
} ngd_str_t;

typedef struct ngd_hnode_s {
    ngd_str_t key;
    ngd_str_t value;
    //
    struct ngd_hnode_s *next;
} ngd_hnode_t;

typedef struct {
    u_char *pos;
    u_char *last;
    off_t *fpos;
    off_t *flast;

    int fd;
    u_char *start;
    u_char *end;
} ngd_buf_t;

typedef enum {
    NGD_OK,
    NGD_AGAIN,
    NGD_DECLINED
} NGD_STATUS;

typedef struct {
    int state;
    //req line
    ngd_str_t method;
    ngd_str_t uri;
    ngd_str_t ver;
    //hlines
    ngd_hnode_t *headers;
    size_t count;
    // //body
    // ndg_req_body_t body;
} ngd_req_t;

const char *RES_OK = "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %d\r\n"
                "\r\n";


typedef struct {
    ngd_list_t headers;
    //cached header;
} ngd_headers_in;


void
ps(void *buf, size_t buflen);

off_t
get_flen(int fd);

void
send_msg(int fd, void *buf, size_t buflen);

void
send_ok(int cli_fd, void *buf, size_t buflen, const char *fname);

ssize_t
recv_msg(int fd, void *buf, size_t buflen);

void
handle_cli(int cli_fd);

void
start();

void
handle_read(int cli_fd, u_char *buf, size_t buflen);


#endif
