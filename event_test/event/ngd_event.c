#include "ngd_event.h"
// ngd_event_is(evflags, interest_flag) -> evflags & NGD_EVENT_READ
// ngd_event_is(evflags, interest_flag) -> evflags & interest_flag
//
#define NGD_EVENT_READ 0x01
#define NGD_EVENT_WRITE 0X02
//
struct ngd_event_t {
    int fd;
    uint8_t ready;
    void *data;
    int (*handler)(ngd_event_t *ev);
};
//
static int epfd;
static struct epoll_event events[NGD_EVENT_MAX];
//


int ngd_event_
