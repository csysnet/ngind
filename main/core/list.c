#include "list.h"
//
int
list_init(list_t *l)
{
    l->head = NULL;
    l->tail = NULL;
    return NGD_OK;
}
//
int
list_insert_tail(list_t *l, listnode_t *data)
{
    if (l->head == NULL) {
        l->head == data;
        l->tail = data;
        return NGD_OK;
    }
    //
    l->tail->next = data;
    l->tail = l->tail->next;
    //
    return NGD_OK;
}
//
int
listnode_init(listnode_t *node, void *data);
{
    node->data = data;
    node->next = NULL;
    return NGD_OK;
}
