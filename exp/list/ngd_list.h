#ifndef NGD_LIST_H
#define NGD_LIST_H

typedef ngd_list_part_s ngd_list_part_t;

struct ngd_list_part_s {
    void *arr;
    ngd_list_part_t *next;
};


typedef struct {
    ngd_pool_t *pool;

    ngd_list_part_t head;
    ngd_list_part_t *last;

    size_t nlimit;//max element per arr
    size_t size; //arr[i] size
} ngd_list_t;



void
ngd_list_init(ngd_list_t list, ngd_pool_t pool, size_t n, size_t size);

void
ngd_list_push(ngd_list_t list);

/*
 *
 *  the iteration through the list:
 *
 *  part = &list.part;
 *  data = part->elts;
 *
 *  for (i = 0 ;; i++) {
 *
 *      if (i >= part->nelts) {
 *          if (part->next == NULL) {
 *              break;
 *          }
 *
 *          part = part->next;
 *          data = part->elts;
 *          i = 0;
 *      }
 *
 *      ...  data[i] ...
 *
 *  }
 */

#endif
