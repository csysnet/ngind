#include "ngd_core.h"


int
main()
{
    int lfd = get_listener(http_init);
    //
    event.add(lfd, lfd_handler);

}

struct listener {
    void (*init_conn)(conn_t *c)
};

lfd_handler(conn *lc)
{
    conn_t *c = conn_accept(lc->fd);
    conn_set_nonblock(c);
    //
    ngd_listen_t *ls = c->data;
    ls->init_conn(c);
    event_regis()
}
