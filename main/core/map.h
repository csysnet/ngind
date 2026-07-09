#ifndef MAP_H
#define MAP_H

#include "core.h"

typedef struct map_t map_t;
typedef struct pool_t pool_t;
typedef struct str_t str_t;

typedef struct mapnode_t {
    str_t *key;
    void *value;
    struct mapnode_t *next;
} mapnode_t;

typedef struct map_t {
    pool_t *pool;
    mapnode_t **buckets;
    size_t bucket_count;
} map_t;

int map_init(map_t *map, pool_t *parent_pool, size_t bucket_count);
int map_insert(map_t *map, str_t *key, void *value);
void *map_get(map_t *map, str_t *key);

#endif
