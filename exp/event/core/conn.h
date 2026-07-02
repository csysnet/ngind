#ifndef CONN_H
#define CONN_H

#include <sys/types.h>

typedef unsigned char u_char;
typedef struct ngd_event_t ngd_event_t;
typedef struct ngd_buf_t ngd_buf_t;
typedef struct ngd_ssl_ctx_t ngd_ssl_ctx_t;

typedef struct ngd_conn_t {
    int fd;
    ssize_t (*recv)(struct ngd_conn_t *c, u_char *buf, size_t size);
    ssize_t (*send)(struct ngd_conn_t *c, u_char *buf, size_t size);

    ngd_event_t *read;
    ngd_event_t *write;

    ngd_buf_t *buf;
    void *ssl;
    void *pdata;

    unsigned closed:1;
} ngd_conn_t;

typedef struct ngd_listening_t {
    int fd;
    ngd_ssl_ctx_t *ssl_ctx;
    int (*handler)(ngd_conn_t *c);
} ngd_listening_t;


ngd_conn_t *ngd_conn_create(int fd);
ngd_conn_t *ngd_conn_listener_create(int port, int backlog, int(*handler)(ngd_conn_t *c));
int ngd_conn_accept(int lfd);
int ngd_conn_close(int fd);
int ngd_conn_set_nonblocking(int fd);



#endif
