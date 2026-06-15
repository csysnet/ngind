#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 64

static void
set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int
main(void)
{
    int listenfd, epfd;
    struct sockaddr_in addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(listenfd, 128);

    set_nonblocking(listenfd);

    epfd = epoll_create1(0);

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;

    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

    struct epoll_event events[MAX_EVENTS];

    printf("Listening on port %d\n", PORT);

    while (1) {

        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);

        for (int i = 0; i < nfds; i++) {

            int fd = events[i].data.fd;

            if (fd == listenfd) {

                /* New connection */

                struct sockaddr_in client;
                socklen_t len = sizeof(client);

                int connfd = accept(listenfd,
                                    (struct sockaddr *)&client,
                                    &len);

                if (connfd == -1)
                    continue;

                set_nonblocking(connfd);

                ev.events = EPOLLIN;
                ev.data.fd = connfd;

                epoll_ctl(epfd,
                          EPOLL_CTL_ADD,
                          connfd,
                          &ev);

                printf("client connected: fd=%d\n", connfd);

            } else {

                /* Existing client has data */

                char buf[1024];

                ssize_t n = read(fd, buf, sizeof(buf));

                if (n <= 0) {

                    printf("client disconnected: fd=%d\n", fd);

                    close(fd);

                    epoll_ctl(epfd,
                              EPOLL_CTL_DEL,
                              fd,
                              NULL);

                    continue;
                }

                write(fd, buf, n); /* echo */
            }
        }
    }

    return 0;
}
