#include "cycle.h"

#include "pool.h"

cycle_t cycle;

int
cycle_init(int listen_cap)
{
    cycle->pool = pool_create();
    cycle->listenings = pool_alloc(sizeof(listening_t) * listen_cap);
    cycle->listen_cap = listen_cap;
    cycle->nlistening = 0;
}


int
cycle_open_listenings(cycle_t *cycle)
{
    for (int i=0; i<cycle->nlistening; i++)
    {
        listening_open(cycle->listenings[i]);
    }

    return NGD_OK;
}

void
cycle_close_listenings(cycle_t *cycle)
{
    for (int i=0; i<cycle->nlistening; i++)
    {
        listening_close(cycle->listenings[i])
    }
}

int
cycle_add_listening(cycle *cycle)
{

}


int
cycle_register_listening_event(cycle *cycle)
{
    ngd_listening_t *ls;
    for (int i=0; i<cycle->nlistening; i++)
    {
        event_register_listening(cycle->listenings[i]);
    }

    return NGD_OK
}
