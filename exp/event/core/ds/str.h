#ifndef STR_T
#define STR_T

typedef unsigned char u_char;

typedef struct str_t {
    u_char *pdata;
    size_t len;
} str_t;

int str_cmp(str_t *s1, str_t *s2);
#endif
