#include "ngd_event.h"
//
static int epfd;
static struct epoll_event events[NGD_EVENT_MAX_GET];
//
static uint8_t
ngd_event_evflags_to_retflags(uint32_t evflags)
{
    uint8_t retflags;
    //
    retflags = 0x00;
    //
    if (evflags & EPOLLIN)
        retflags |= NGD_EVENT_READ;
    if (evflags & EPOLLOUT)
        retflags |= NGD_EVENT_WRITE;
    if (evflags & EPOLLHUP)
        retflags |= NGD_EVENT_HUP;
    if (evflags & EPOLLERR)
        retflags |= NGD_EVENT_ERR;
    //
    return retflags;
}
static int
ngd_event_set(ngd_conn_t *c, int op, int evflags)
{

}
//
void
ngd_event_module_init(void)
{
    epfd = epoll_create1(0);
}
int
ngd_event_module_loop(int timeout)
{
    int n;
    uint8_t retflags;
    ngd_event_t *ev;
    uint32_t evflags;
    //
    n = epoll_wait(epfd, events, sizeof(events), timeout);
    retflags = 0x00;
    //
    for (int i=0; i<n; i++)
    {
        ev = events[i].data.ptr;
        evflags = events[i].events;
        //

    }
}
//
int ngd_event_regis(ngd_event_t *ev,
                    int fd,
                    void (*handler)(ngd_event_t *ev),
                    void *data);
int ngd_event_unregis(ngd_event_t *ev);
uint8_t ngd_event_get_flags(ngd_event_t *ev);
