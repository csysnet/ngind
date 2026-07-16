#include "str.h"
#include "string.h"
//
int
str_from_chars(str_t *s, const char *chars)
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

void
ps(str_t *s)
{

    // printf("keylen: %lu\n", s->len);
    for (int i = 0; i<s->len; i++)
        printf("%c", s->p[i]);
}
