#ifndef NGD_LISTENING_H
#define NGD_LISTENING_H

typedef struct ngd_ssl_ctx_t ngd_ssl_ctx_t;
typedef struct ngd_cycle_t cycle;

typedef struct ngd_listening_t {
    int fd;
    struct sockaddr_in addr;
    socklen_t socklen;

    int backlog;

    ngd_ssl_ctx_t *ssl_ctx;

    int (*handler)(ngd_conn_t *c);
} ngd_listening_t;

ngd_listening_t *ngd_create_listening(uint16_t port);
int ngd_open_listening_sockets(ngd_cycle_t *cycle);
void ngd_close_listening_sockets(ngd_cycle_t *cycle);


#endif
