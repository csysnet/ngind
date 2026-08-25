#ifndef NGD_STR_T
#define NGD_STR_T
//
#include "ngd_core.h"
//note!!!
#define NGD_STR(s) \
    (ngd_str_t){ .data = (s), .len = sizeof(s) - 1 }
//
typedef struct ngd_str_t ngd_str_t;
//
struct ngd_str_t {
    void *data;
    size_t len;
};
//
int str_cmp(str_t *s1, str_t *s2);
void str_zeros(void *data, size_t len);
ngd_str_print(ngd_str_t *s);
//
//
#endif
