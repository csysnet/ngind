#ifndef EVENT_H
#define EVENT_H

typedef struct listen_t listen_t;

typedef struct event_t {
    void *pdata
    int (*handler)(struct event_t *ev);
} event_t;

int event_init(void);
int event_loop(void);

int event_regis_listen(listen_t *ls);


#endif
