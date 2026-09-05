#ifndef NGD_LIST_H
#define NGD_LIST_H
//
#include "ngd_core.h"
//
typedef struct ngd_list_node_t ngd_list_node_t;
typedef struct ngd_list_t ngd_list_t;
//
struct ngd_list_node_t {
    void *data;
    ngd_list_node_t *next;
};
//
struct ngd_list_t {
    ngd_list_node_t *head;
    ngd_list_node_t *tail;
    ngd_pool_t *pool;
};
//
void ngd_list_init(ngd_list_t *l, ngd_pool_t *pool);
int ngd_list_append(ngd_list_t *l, void *data)
//
void
ngd_list_init(ngd_list_t *l, ngd_pool_t *pool)
{
    l->head = NULL;
    l->tail = NULL;
    l->pool = pool;
}
//
int
ngd_list_append(ngd_list_t *l, void *data)
{
    ngd_list_node_t *node;
    //
    node = ngd_pool_alloc(l->pool, sizeof(*node));
    if (node == NULL)
        return NGD_ERR;
    node->data = data;
    node->next = NULL;
    //
    if (l->head == NULL) {
        l->head = node;
        l->tail = node;
    } else {
        l->tail.next = node;
    }
    //
    return NGD_OK;
}
/*
ngd_list_loop()
{
    ngd_list_node_t *p;
    for (p=l->head; p != NULL; p->next)
    {
        do something
    }
}
 */

#endif
