#ifndef NGD_STR_T
#define NGD_STR_T
//
#include "ngd_core.h"
//note!!!
#define NGD_STR_C(s) \
    (ngd_str_t){ .data = (s), .len = sizeof(s) - 1 }
#define NGD_STR(data, len) \
    (ngd_str_t){ .data = (data), .len= (len)}
//
typedef struct ngd_str_t ngd_str_t;
//
struct ngd_str_t {
    void *data;
    size_t len;
};
//
void ngd_str_cpy(str_t src_str, str_t dest_str)
bool ngd_str_iequal(str_t s1, str_t s2);
void ngd_str_zeros(str_t s);
void ngd_str_print(ngd_str_t s);
//
//
#endif
