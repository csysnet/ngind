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
