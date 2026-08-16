#ifndef NGD_REQ_H
#define NGD_REQ_H
//
typedef struct ngd_req_t ngd_req_t;
//
struct {
    int state;
    //request line
    str_t *smethod;
    str_t *suri;
    str_t *sver;
    u_char *method_start;
    u_char *method_end;
    u_char *uri_start;
    u_char *uri_end;
    u_char *
}

#endif
