#include "http.h"
#include "conn.h"
#include "req.h"
#include "pool.h"
#include "event.h"


int
http_init_conn(conn_t *c)
{
    //
    c->read->pdata = c;
    c->read->handler = http_wait_req;
    c->write->pdata = c;
    c->write->handler = http_empty_handler;
    //
    return NGD_OK;
}
