#include <stdio.h>

#include "core/core.h"
#include "http/http.h"


#include <stdio.h>

#define PORT 8083

int
main(int argc, char *argv[])
{
    ngd_listening_t *ls;
    ngd_cycle_t *cycle;

    ls = ngd_create_listening(PORT, ngd_http_init_conn);

    cycle = ngd_create_cycle();
    cycle->listening[0] = ls;
    cycle->nlistening++;

    ngd_open_listening_sockets(cycle);

    ngd_event_init();
    ngd_event_proc_init(cycle);
    ngd_event_loop();
}


int
main(int argc, char *argv[])
{
    ngd_listening_t *ls;
    ngd_cycle_t *cycle;

    ls = ngd_create_listening(PORT, ngd_http_init_conn);

    cycle = ngd_create_cycle();
    cycle->listening[0] = ls;
    cycle->nlistening++;

    ngd_open_listening_sockets(cycle);
    event_add_listening(http);

    ngd_event_init();
    ngd_event_proc_init(cycle);
    ngd_event_loop();
}

int main(void)
{
    event_init();

    /* HTTP */
    listen_t *http = listen_create("0.0.0.0", 80);
    http->app_init = http_init_connection;
    http->ssl = false;
    cycle_add_listening(80, http_init_connection, NULL);

    /* HTTPS */
    listen_t *https = listen_create("0.0.0.0", 443);
    https->app_init = http_init_connection;
    https->ssl = true;
    https->ssl_ctx = ssl_ctx_create(...);
    event_add_listening(https);

    event_loop();

    return 0;
}

void
event_accept(event_t *ev)
{
    conn_t *c;

    c = conn_accept(ev);

    if (c->listening->ssl_ctx) {

        ssl_create_connection(c);
        c->read->handler = ssl_handshake;

        return;
    }

    c->listening->app_init(c);
}
