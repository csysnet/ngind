#ifndef SOCK_H
#define SOCK_H

typedef struct {
    int fd;



    req_t req;
} conn_t;

void
start(

)
{
    int listen_fd
    int n_read;
    struct sockaddr_in addr;

    serv_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(serv_fd, (struct sockaddr *)&addr, sizeof(addr));

    listen(serv_fd, BACKLOG);

    while (1) {
        cli_fd = accept(serv_fd, (struct sockaddr *)NULL, NULL);

        handle_cli(cli_fd);
        close(cli_fd);
    }
}


#endif;
