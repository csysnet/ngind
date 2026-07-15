#include "str.h"
#include "string.h"
//
int
str_cmp(str_t *s1, str_t *s2)
{
    int rc;
    rc = memcmp(s1, s2, s1->len);

    printf("reach string cmp\n");
    return rc;
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
    printf("\n");
}
