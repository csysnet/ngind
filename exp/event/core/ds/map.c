#include "map.h"


map_t *
map_create(pool_t *pool, size_t bucket_count)
{
    map_t *map;

    map = pool_alloc(pool, sizeof(map_t));
    map->pool = pool;
    map->buckets = pool_calloc(pool, bucket_count * sizeof(map_node_t *));
    map->bucket_count = bucket_count;
    return map;
}

int
map_insert(map_t *map, str_t *key, void *value)
{

}
