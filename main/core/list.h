#ifndef LIST_H
#define LIST_H

#include "core.h"
//
typedef struct listnode_t listnode_t;
typedef struct list_t list_t;
//
struct listnode_t {
    void *data;
    listnode_t *next;
};
//
struct list_t {
    listnode_t *head;
    listnode_t *tail;
};
//
int list_init(list_t *l);
int list_insert_tail(list_t *l, listnode_t *data);
int listnode_init(listnode_t *node, void *data);


#endif
