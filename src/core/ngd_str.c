#include "string.h"
#include "stdarg.h"
//
#include "ngd_str.h"
//
void
ngd_str_cpy(u_char *dest, u_char *src, size_t n)
{

}
bool
ngd_str_isin(ngd_str_t s1, ngd_str_t s2)
{
    return true;
}


bool
ngd_str_iequal(ngd_str_t s1, ngd_str_t s2)
{
    return true;

}
bool
ngd_str_equal(ngd_str_t s1, ngd_str_t s2)
{
    return true;
}
void
ngd_str_zeros(u_char *buf, size_t len)
{
    memset((void *)buf, 0,len);
}
void
ngd_str_print(ngd_str_t s)
{
    for (size_t i=0; i < s.len ; ++i)
        printf("%c", s.data[i]);
    printf("\n");
}

int
ngd_str_snprintf(u_char *buf, size_t size, size_t *bytes_written,
    const char *fmt, ...)
{
    va_list ap;
    int     n;

    va_start(ap, fmt);

    n = vsnprintf((char *)buf, size, fmt, ap);

    va_end(ap);

    if (n < 0) {
        return NGD_ERR;
    }

    if ((size_t) n >= size) {
        return NGD_ERR;
    }

    *bytes_written = (size_t) n;

    return NGD_OK;
}
