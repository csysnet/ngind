#ifndef NGD_LIST_H
#define NGD_LIST_H
//
#include "ngd_core.h"
//
typedef struct ngd_list_node_t ngd_list_node_T;
typedef struct ngd_list_t ngd_list_t;
//
struct ngd_list_t {
    ngd_list_node_t *head;
    ngd_pool_t *pool;
};
//
void ngd_list_init(ngd_list_t *l, ngd_pool_t *pool);
void ngd_list_push(ngd_list_t *l, void *data)

#endif
