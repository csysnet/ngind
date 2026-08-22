#ifndef NGD_EPOLL_H
#define NGD_EPOLL_H
//
#include "sys/epoll.h"
//
#define NGD_EPOLL_MAXEVENTS 1024 //size of ready array
//
#define NGD_READABLE EPOLLIN
#define NGD_WRITABLE EPOLLOUT
#define NGD_HUP EPOLLHUP
//
void ngd_epoll_module_init(void);
int ngd_epoll_add(int fd, void *data, uint32_t interest_flag);
int ngd_epoll_mod(int fd, void *data, uint32_t interest_flag);
int ngd_epoll_del(int fd);
int ngd_epoll_wait(int timeout);
//get context from ready array
void *ngd_epoll_get_data(int index);
uint32_t ngd_epoll_get_actual_flag(int index);
//
#end

how to use epoll

for (;;)
{
    int timer = get_next_timer();
    int n = epoll_wait(timer);
    for (int i=0; i<n; i++)
    {
        conn_t *c = epoll_events[i].data.ptr;
    }
}
