#ifndef NGD_STR_H
#define NGD_STR_H
//
#include "ngd_core.h"
//
#define NGD_STR_C(s) \
    (ngd_str_t){ .data = s, .len = sizeof(s) - 1 }
//
typedef struct ngd_str_t ngd_str_t;
//
struct ngd_str_t {
    u_char *data;
    size_t len;
};
//
void ngd_str_cpy(u_char *dest, u_char *src, size_t n);
void ngd_str_zeros(u_char *buf, size_t len);
void ngd_str_log(const char *fmt, ...);
void ngd_str_print(ngd_str_t s);
int ngd_str_snprintf(
    u_char *buf,
    size_t size,
    size_t *bytes_written,
    const char *fmt, ...
);
bool ngd_str_endwith(ngd_str_t ends, ngd_str_t s);
bool ngd_str_isin(ngd_str_t s1, ngd_str_t s2);
bool ngd_str_iequal(ngd_str_t s1, ngd_str_t s2);
bool ngd_str_equal(ngd_str_t s1, ngd_str_t s2);
//
#endif
