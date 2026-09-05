#include "conn.h"
#include "ev.h"
#include "http.h"
#include <stdlib.h>


int
main()
{
    ngd_event_init();
    ngd_conn_t *lc = ngd_conn_listener_create(8080, 10, ngd_http_init_conn);
    lc->read = ngd_event_accept;
    ngd_event_add_conn(lc);
    ngd_event_loop();
}



// if (!c->closed && (revents & EPOLLOUT))
//     c->write->handler(c->write);

// // int main()
// // {
// // event_init(&cycle);

// // http_init(&cycle); // explicit call

// // event_loop_run(&cycle);
// // }

// int
// main()
// {
//    ngd_event_init()


//     epfd = ngd_event_create_epoll();
//     ngd_event_init(epfd);
//     ngd_event_proc(epfd);
// }

// int main()
// {



//     event_init(&cycle);

//     ngd_event_init();

//     ngd_conn_t *lc = malloc(sizeof(lc));
//     lc->fd = ngd_conn_create_lfd();
//     ngd_http_init(lc);
//     ngd_event_add_conn(lc);

//     http_init(&cycle); // explicit call

//     event_loop_run(&cycle); //just notify on registered fd
// }


// /*
//  * init the event meta
//  * setup http thing by calling ngd_http_init
//  */


// void
// http_init(ngd_conn_t *lc)
// {
//     ngd_event_t *rev = malloc(sizeof(ev));

//     rev = lc->read;
//     rev->pdata = lc;
//     rev->handler = ngd_event_accept
// }



// int main(void)
// {
//     Server server;

//     server_init(&server);

//     server_run(&server);

//     return 0;
// }


// void server_init(Server *s)
// {
//     event_init();

//     Listener *http = listener_create(8080);

//     http->init_connection = http_init_connection;

//     register_listener(http);
// }


// void server_run(Server *s)
// {
//     event_loop();
// }


// main()
//     ↓
// server_init()
//     ↓
// event_init()
//     ↓
// listener_create()
//     ↓
// register_listener()
//     ↓
// server_run()
//     ↓
// event_loop()


// void register_listener(Listener *ls)
// {
//     Connection *c;

//     c = connection_create(ls->fd);

//     c->data = ls;

//     c->read.handler = accept_handler;

//     epoll_add(ls->fd, EPOLLIN, c);
// }
