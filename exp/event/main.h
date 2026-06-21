#ifndef MAIN_H
#define MAIN_H
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <string.h>
#define PORT 8080
#define MAX_EVENTS 64
#define BACKLOG 10

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

typedef struct ngd_event_t {
    void *pdata;
    int (*handler)(struct ngd_event_t *ev);
} ngd_event_t;

typedef struct ngd_conn_t {
    int fd;
    ssize_t (*recv)(struct ngd_conn_t *c,
                       u_char *buf,
                       size_t size);



    ngd_event_t *read;
    ngd_event_t *write;
    ngd_buf_t *buf;
    void *pdata;
} ngd_conn_t;



#endif
