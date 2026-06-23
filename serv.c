// epoll_server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define PORT 8000
#define MAX_EVENTS 64
#define BUF_SIZE 1024

// set fd non-blocking
int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int server_fd, epfd;
    struct sockaddr_in addr;

    // create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    set_nonblocking(server_fd);

    // bind
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, SOMAXCONN);

    // create epoll
    epfd = epoll_create1(0);

    struct epoll_event ev, events[MAX_EVENTS];

    // add server socket
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);


    while (1) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);

        for (int i = 0; i < n; i++) {
    printf("Listening on port %d...\n", PORT);
            int fd = events[i].data.fd;

            if (fd == server_fd) {
                // accept new connection
                int client_fd = accept(server_fd, NULL, NULL);
                set_nonblocking(client_fd);

                const char *msg = "Welcome!\n";
                write(client_fd, msg, strlen(msg));

                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);

            } else {
                // handle client
                char buf[BUF_SIZE];
                int count = read(fd, buf, sizeof(buf));

                if (count <= 0) {
                    close(fd);
                } else {
                    // echo back
                    write(fd, buf, count);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
