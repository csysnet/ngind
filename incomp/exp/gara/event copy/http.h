#ifndef HTTP_H
#define HTTP_H
#include "ev.h"

int ngd_http_init_conn(ngd_conn_t *c);
int ngd_http_proc_reqline(ngd_event_t *ev);
int ngd_http_proc_headers(ngd_event_t *ev);
int ngd_http_proc_body(ngd_event_t *ev);
int ngd_http_conn_switch(ngd_event_t *ev);
int ngd_http_build_res(ngd_event_t *ev);

#endif
