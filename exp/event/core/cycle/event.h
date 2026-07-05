#ifndef EVENT_H
#define EVENT_H

#include "core.h"

#define MAX_EVENTS 511
#define EVENT_WAITTIME -1
typedef struct listen_t listen_t;

typedef struct event_t {
    void *pdata
    int (*handler)(struct event_t *ev);
} event_t;
//
int event_init(void);
int event_regis_listen(listen_t *ls);
int event_accept(event_t *rev);
//
int event_enable_write(event_t *wev);
int event_disable_write(event_t *wev);
int event_del(event *ev);
//
int event_loop(void);


#endif
