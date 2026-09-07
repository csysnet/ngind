#ifndef NGD_STR_H
#define NGD_STR_H
//
#include "ngd_core.h"
//
#define NGD_STR_C(s) \
    (ngd_str_t){ .data = s, .len = sizeof(s) - 1 }
#define NGD_STR(data, len) \
    (ngd_str_t){ .data = (data), .len = (len) }
//
typedef struct ngd_str_t ngd_str_t;
//
struct ngd_str_t {
    void *data;
    size_t len;
};
//
void ngd_str_cpy(u_char *dest, u_char *src, size_t n);
bool ngd_str_iequal(ngd_str_t s1, ngd_str_t s2);
int ngd_str_to_size(ngd_str_t slen, size_t *size);
int ngd_str_from_size(size_t size, ngd_str_t *slen);
void ngd_str_zeros(u_char *buf, size_t len);
void ngd_str_print(ngd_str_t s);
int ngd_str_snprintf(
    char *str,
    size_t size,
    size_t *bytes_written,
    const char *fmt, ...
);
//
#endif


int
ngd_str_snprintf(char *str, size_t size, size_t *bytes_written,
    const char *fmt, ...)
{
    va_list ap;
    int     n;

    va_start(ap, fmt);

    n = vsnprintf(str, size, fmt, ap);

    va_end(ap);

    if (n < 0) {
        return NGD_ERR;
    }

    /*
     * n is the number of bytes that would have been written,
     * excluding the terminating '\0'.
     */

    if ((size_t) n >= size) {
        return NGD_ERR;
    }

    *bytes_written = (size_t) n;

    return NGD_OK;
}
