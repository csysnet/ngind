#include "stdlib.h"
//
#include "str.h"
#include "string.h"
#include "pool.h"
//
int
str_from_chars(str_t *s, char *chars)
{
    s->p = chars;
    s->len = strlen(chars);
    return NGD_OK;
}



int
str_cmp(str_t *s1, str_t *s2)
{
    return memcmp(s1->p, s2->p, s2->len);
}
int
str_zeros(u_char *pdata, size_t len)
{
    return NGD_OK;
}

int
ngd_str_to_size(str_t *s, size_t *size)
{
    size_t n = 0;
    char c;

    for (size_t i = 0; i < s->len; i++)
    {
        c = s->data[i];
        if (c < '0' || c > '9')
            return NGD_ERR;

        if (n > (SIZE_MAX - (c - '0')) / 10)
            return NGD_ERR;

        n = n * 10 + (c - '0');
        s++;
    }
    *size = n;
    return NGD_OK;
}

int
str_to_long(pool_t *pool, str_t *s)
{
    char *chars;
    //
    chars = pool_alloc(pool, s->len + 1);
    memcpy(chars, s->p, s->len);
    chars[s->len] = '\0';
    //
    return strtol(chars, NULL, 10);
}


void
ps(str_t *s)
{

    // printf("keylen: %lu\n", s->len);
    for (int i = 0; i<s->len; i++)
        printf("%c", s->p[i]);
}
