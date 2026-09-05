#ifndef EV_H
#define EV_H


#include "conn.h"
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
#define BACKLOG 10
#define MAX_EVENTS 64



void
ngd_event_accept(ngd_event_t *ev)
{
    ngd_conn_t *lc = ev->conn;
    ngd_listening_t *ls = (ngd_listening_t *)lc->pdata;

    int fd = accept(ls->fd, NULL, NULL);
    ngd_conn_t *c = connection_create(fd);

    ls->
}


// int
// ngd_event_init(int epfd)
// {
//     ngd_conn_t *lc;
//     ngd_event_t *ev;

//     lc = malloc(sizeof(ngd_conn_t));
//     lc->fd = ngd_conn_create_lfd();
//     lc->read->handler = ngd_event_accept;
//     lc->read->conn = lc;

//     ev = malloc(sizeof(ngd_event_t));
//     ev->conn = lc;
//     ev->handler = ngd_event_accept;

//     ngd_event_add(epfd, ev);

//     return 0;
// }

typedef struct {
    int max_events;
    // connecti

} ngd_cycle_t;


typedef struct {
    event_loop_t event_loop;

    connection_pool_t conns;

    listening_t *listening;
} cycle_t;

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
    // unsigned active:1;
    // unsigned write:1;
} ngd_event_t;

int ngd_event_add_conn(ngd_conn_t *c);

int ngd_event_init();
int ngd_event_loop();
//
int ngd_event_accept(ngd_event_t *rev);
// int ngd_event_add(ngd_event_t)
//







int ngd_event_add(ngd_event_t *ev);
int ngd_event_enable_write(int epfd, ngd_event_t *ev);
int ngd_event_disable_write(int epfd, ngd_event_t *ev);
int ngd_event_del(int epfd, ngd_event_t *ev);
#endif
