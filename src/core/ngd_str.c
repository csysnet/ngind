#include "string.h"
#include "stdarg.h"
//
#include "ngd_str.h"
//
static u_char
ngd_str_tolower(u_char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + ('a' - 'A');
    //
    return c;
}

//
void
ngd_str_cpy(u_char *dest, u_char *src, size_t n)
{
    memcpy(dest, src, n);
}
void
ngd_str_zeros(u_char *buf, size_t len)
{
    memset((void *)buf, 0,len);
}
void
ngd_str_log(const char *fmt, ...)
{
    va_list args;
    //
    fprintf(stderr, NGD_BLUE"[LOG]"NGD_RESET " ");
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    //
    fprintf(stderr, "\n");
}

void
ngd_str_print(ngd_str_t s)
{
    for (size_t i=0; i<s.len; i++)
        fprintf(stderr, "%c", s.data[i]);
}

int
ngd_str_snprintf(u_char *buf, size_t size, size_t *bytes_written,
    const char *fmt, ...)
{
    va_list args;
    int     n;

    va_start(args, fmt);

    n = vsnprintf((char *)buf, size, fmt, args);

    va_end(args);

    if (n < 0) {
        return NGD_ERR;
    }

    if ((size_t) n >= size) {
        return NGD_ERR;
    }

    *bytes_written = (size_t) n;

    return NGD_OK;
}

bool
ngd_str_isin(ngd_str_t s1, ngd_str_t s2)
{
    if (s1.len == 0 || s2.len == 0 || s2.len < s1.len)
        return false;
    //
    for (size_t i=0; i <= s2.len - s1.len; ++i)
        if (memcmp(s2.data + i, s1.data, s1.len) == 0)
            return true;
    //
    return false;
}
bool
ngd_str_iequal(ngd_str_t s1, ngd_str_t s2)
{
    if (s1.len != s2.len)
        return false;
    //
    for (size_t i = 0; i < s1.len; i++)
        if (ngd_str_tolower(s1.data[i]) != ngd_str_tolower(s2.data[i]))
            return false;

    //
    return true;
}
bool
ngd_str_equal(ngd_str_t s1, ngd_str_t s2)
{
    if (s1.len != s2.len)
        return false;
    //
    return memcmp(s2.data, s1.data, s1.len) == 0;
}
