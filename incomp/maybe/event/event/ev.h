#ifndef EV_H
#define EV_H

typedef struct ngd_conn_t ngd_conn_t;
typedef struct ngd_listening_t ngd_listening_t;

typedef struct event_loop_t {
    int epfd;
    struct epoll_event *events;
    int nevents;
} event_ctx_t;


typedef struct ngd_event_t {
    void *pdata;
    int (*handler)(struct ngd_event_t *ev);
    unsigned active:1;
    // unsigned write:1;
} ngd_event_t;

int ngd_event_init(void);
int ngd_event_loop(void);

int ngd_event_register_conn(ngd_conn_t *c);

int ngd_event_accept(ngd_event_t *rev);
int ngd_event_enable_write(ngd_event_t *wev);
int ngd_event_disable_write(ngd_event_t *wev);
int ngd_event_del(ngd_event_t *ev);
int ngd_event_proc_init(ngd_cycle *cycle);

#endif
