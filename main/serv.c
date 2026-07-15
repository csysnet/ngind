#include <stdio.h>

#include "cycle.h"
#include "http.h"

#define PORT1 8080

int
main() {
    cycle_t *cycle;
    cycle = cycle_create(1);
    cycle_add_listen(cycle, PORT1, 0, http_init_conn);
    cycle_init_event();
    cycle_open_listens(cycle);
    cycle_register_listens(cycle);
    cycle_event_loop();
};
