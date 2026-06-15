#ifndef NGD_EVENT_H
#define NGD_EVENT_H

typedef struct ngd_event_t {
    void *pdata;
    void (*handler)(struct ngd_event_t *ev);
} ngd_event_t;

#endif
