#ifndef STR_T
#define STR_T

#include "core.h"
//
typedef struct pool_t pool_t;
typedef struct str_t {
    u_char *p;
    size_t len;
} str_t;

int str_from_chars(str_t *s, char *chars);

int str_cmp(str_t *s1, str_t *s2);
int str_zeros(u_char *pdata, size_t len);
int str_to_long(pool_t *pool, str_t *s);
void ps(str_t *s);
#endif
