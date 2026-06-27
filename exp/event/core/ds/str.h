#ifndef NGD_STR_T
#define NGD_STR_T

typedef struct ngd_str_t {
    u_char  *pdata;
    size_t len;
} ngd_str_t;

int ngd_str_cmp(ngd_str_t s1, ngd_str_t s2)


#endif
