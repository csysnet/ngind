#ifndef NGD_DICT_H
#define NGD_DICT_H

typedef struct ngd_str_t ngd_str_t

typedef struct ngd_pair_t {
    ngd_str_t *key;
    ngd_str_t *value;
    struct ngd_pair_t *next;
} ngd_pair_t;

typedef struct ngd_dict_t {
    ngd_pair_t **buckets;

} ngd_dict_t;


#endif
