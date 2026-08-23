#ifndef NGD_EVENT_H
#define NGD_EVENT_H
//
#include "ngd_core.h"
//
#define NGD_EVENT_MAX_GET 1024
#define NGD_EVENT_READ 0x01
#define NGD_EVENT_WRITE 0x02
#define NGD_EVENT_HUP 0x04
#define NGD_EVENT_ERR 0x08
//DON'T USE MEMBER DIRECTLY
typedef struct ngd_event_t ngd_event_t;
//
struct ngd_event_t {
    int fd;
    uint8_t flags;
    void (*handler)(ngd_event_t *ev);
    void *data;
};
//
void ngd_event_module_init(void);
int ngd_event_module_loop(int timeout);
//
int ngd_event_regis(ngd_event_t *ev,
                    int fd,
                    void (*handler)(ngd_event_t *ev),
                    void *data);
int ngd_event_unregis(ngd_event_t *ev);
uint8_t ngd_event_get_flags(ngd_event_t *ev);
//
#endif
