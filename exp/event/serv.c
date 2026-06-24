#include <stdio.h>

#include "core/core.h"
#include "http/http.h"


#include <stdlib.h> // malloc
#include <string.h> // memset
#include <unistd.h> // close
#include <sys/socket.h> // socket, bind, listen, accept
#include <netinet/in.h> // sockaddr_in, htons
#include <stdio.h>

#define PORT 8083
#define BACKLOG 124


int
main(int argc, char *argv[])
{
    ngd_event_init();
    ngd_conn_t *lc = ngd_conn_listener_create(PORT, BACKLOG, ngd_http_init_conn);
    ngd_event_register_conn(lc);
    ngd_event_loop();

}

int
main()
{
    ngd_cycle_t *cycle = malloc(sizeof(ngd_cycle_t))

    ngd_cycle_open_listening_sockets(cycle);

}


ngd_cycle = ngd_init_cycle();

ls = ngd_create_listening(INADDR_ANY, 80);
ls->handler = ngd_http_init_connection;

cycle->listening[0] = ls;
cycle->listening_n = 1;

ngd_open_listening_sockets(cycle);

ngd_event_init();

for (i = 0; i < cycle->listening_n; i++) {

    c = ngd_get_connection(ls->fd);

    c->listening = ls;

    c->read->handler = ngd_event_accept;

    ngd_event_add(c->read);
}

ngd_process_events();
