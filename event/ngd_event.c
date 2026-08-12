#include "ngd_event.h"
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
