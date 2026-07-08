#include "conn.h"
#include "req.h"
#include "pool.h"



int
http_init_conn(conn_t *c)
{
    //
    c->read->handler = http_wait_req;
    c->read->pdata = c;
    c->write->handler = http_empty_handler;
    c->write->handler = c;
    //
    return NGD_OK
}
