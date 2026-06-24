#ifndef NGD_CYCLE_H
#define NGD_CYCLE_H

typedef struct {

    ngd_listening_t **listenings;
    int nlistening;
} ngd_cycle_t;

int ngd_init_cycle(cycle);


#endif
