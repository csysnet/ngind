#ifndef CONN_H
#define CONN_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include "ev.h"

typedef struct ngd_conn_t {
    int fd;
    ssize_t (*recv)(struct ngd_conn_t *c,
                       u_char *buf,
                       size_t size);



    ngd_event_t *read;
    ngd_event_t *write;
    ngd_buf_t *buf;
    void *pdata;
    int closed;
} ngd_conn_t;

typedef struct {
    int fd;
    int (*handler)(ngd_conn_t *c);
} ngd_listener_t;

ngd_conn_t *ngd_conn_create(int fd);
ngd_conn_t *ngd_conn_listener_create(int port, int backlog, int(*handler)(ngd_conn_t *c));
int ngd_conn_accept(int lfd);
int ngd_conn_close(int fd);


#endif
