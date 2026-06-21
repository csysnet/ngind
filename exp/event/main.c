#include "main.h"

int create_lc() {
    struct sockaddr_in addr;
    int listen_fd;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    memset(&addr, 0, sizeof(struct sockaddr_in));

    bind(listen_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
    listen(listen_fd, BACKLOG);

    return listen_fd;
}

int
ngd_event_accept(ngd_event_t *rev)
{
    ngd_conn_t *lc = rev->pdata;


    accept(lc->fd, (struct sockaddr *)NULL, NULL);

    return 0;
}


int
main()
{
    int listen_fd, epfd;
    struct epoll_event ev;
    ngd_conn_t lc;
    lc.fd = create_listen_fd();
    lc.read->handler = ngd_accept_conn;


    epfd = epoll_create1(0);
    ev.events = EPOLLIN;
    ev.data.ptr = c;





}
