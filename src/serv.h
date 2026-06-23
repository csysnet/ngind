#ifndef SERV_H
#define SERV_H

#include <sys/types.h>

// #define PORT 8080
#define PORT 8081
// #define PORT 8082
#define BACKLOG 10
#define BUFF_SIZE 16192

typedef unsigned char u_char;

// typedef struct {
//     u_char  *p;
//     size_t len;
// } ngd_str_t;

// typedef struct ngd_event_t {
//     void *pdata;
//     void (*handler)(struct ngd_event_t *ev);
// } ngd_event_t;


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

// typedef enum {
//     NGD_OK,
//     NGD_AGAIN,
//     NGD_ERR,
//     NGD_DECLINED
// } NGD_STATUS;

// typedef struct {
//     ngd_str_t key;
//     ngd_str_t value;
// } hline;

// typedef struct {
//     //req line
//     ngd_str_t method;
//     ngd_str_t uri;
//     ngd_str_t ver;

//     hline headers[64];
//     size_t hpos;

//     //owned by parser
//     //request line
//     int state;

//     // //body
//     // ndg_req_body_t body;
// } ngd_req_t;

const char *RES_OK = "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %d\r\n"
                "\r\n";


// typedef struct {

// } ngd_event_t;

void ps(void *buf, size_t buflen);

off_t get_flen(int fd);

// void send_msg(int fd, void *buf, size_t buflen);

void send_ok(int cli_fd, void *buf, size_t buflen, const char *fname);

// ssize_t recv_msg(int fd, void *buf, size_t buflen);

void handle_cli(int cli_fd);

void start();

// void handle_read(int cli_fd, ngd_buf_t *buf);


// int ngd_proc_reqline(ngd_event_t);

// int ngd_parse_reqline()

// int ngd_lex_header(ngd_req_t *r, ngd_buf_t *buf);
// int ngd_lex_body


#endif
