#ifndef MAP_H
#define MAP_H
#include <stddef.h>

#include "pool.h"
#include "str.h"

typedef struct map_node_t {
    str_t *key;
    void *value;
    struct map_node_t *next;
} map_node_t;

typedef struct map_t {
    pool_t *pool;
    map_node_t **buckets;
    size_t bucket_count;
} map_t;

map_t *map_create(map_t *map, pool_t *pool, size_t bucket_count);
int map_insert(map_t *map, str_t *key, void *value);
void *map_find(map_t *map, str_t *key);
#endif
