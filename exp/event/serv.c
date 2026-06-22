#include <stdio.h>

#include "core/core.h"
#include "http/http.h"

#define PORT 8080
#define BACKLOG 10
int
main()
{
    ngd_event_init();
    ngd_conn_t *lc = ngd_conn_listener_create(PORT, BACKLOG, ngd_http_init_conn);
    ngd_event_register_conn(lc);

    ngd_event_loop();
}
