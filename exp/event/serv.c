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
