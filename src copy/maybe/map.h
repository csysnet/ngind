#ifndef MAP_H
#define MAP_H
this hash is insensitive, key1=dat,key2=DAT, key1=key2
#include "core.h"

#define MAP_BUCKET_COUNT 1


typedef struct map_t map_t;
typedef struct pool_t pool_t;
typedef struct str_t str_t;

typedef struct mapnode_t {
    uint32_t hash
    str_t *key;
    void *value;
    struct mapnode_t *next;
} mapnode_t;

typedef struct map_t {
    pool_t *pool;
    mapnode_t **buckets;
    size_t bucket_count;
} map_t;

int map_init(map_t *map, size_t bucket_count, pool_t *parent_pool);
int map_insert(map_t *map, str_t *key, void *value);
void *map_find(map_t *map, str_t key);

#endif
