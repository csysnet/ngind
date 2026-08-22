#ifndef NGD_EVENT
#define NGD_EVENT
//
#include "ngd_core.h"
//
#define NGD_EVENT_MAX 1024
#define NGD_EVENT_WAIT_TIME -1
//
typedef struct {

}
//
int ngd_event_module_init(void);
int ngd_event_regis(int fd,
                void *data,
                int (*handler)(ngd_event_t *ev));
int ngd_event_unregis(ngd_event_t *ev);
//
int ngd_event_enable_write(ngd_event_t *ev);
int ngd_event_disable_write(ngd_event_t *ev)
//
bool ngd_event_is_read(ngd_event_t *ev);
bool ngd_event_is_write(ngd_event_t *ev);
void *ngd_event_get_data(ngd_event_t *ev);
//
#endif
