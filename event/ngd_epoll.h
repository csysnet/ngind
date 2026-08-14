#ifndef NGD_EPOLL_H
#define NGD_EPOLL_H
//
#define NGD_EPOLL_READ
#define NGD_EPOLL_WRITE
#define NGD_EPOLL_HUP
#define NGD_EPOLL_MAXEVENTS 1024
//
static int epfd;
static struct epoll_event[NGD_EPOLL_MAXEVENTS]
//
int ngd_epoll_regis(fd, interest_flag);
int ngd_epoll_unregis(...);
int ngd_epoll_loop(...);

void ngd_epoll_module_init(void);
int ngd_epoll_add(int fd, 

int process_events_and_timers(events...)
{
    for (;;)
    {

        int n = process_events()
    }
}
//
#end
