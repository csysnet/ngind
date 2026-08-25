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
void ngd_str_cpy(void *src, size_t src_len, void *dest, size_t *dest_len);
bool ngd_str_equal(str_t *s1, str_t *s2);
void ngd_str_zeros(void *data, size_t len);
void ngd_str_print(ngd_str_t *s);
//
//
#endif
