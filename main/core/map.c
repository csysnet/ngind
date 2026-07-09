#include "map.h"
#include "pool.h"
#include "str.h"

static size_t
hash(str_t *key)
{
    return 0;
}

int
map_init(map_t *map, pool_t *parent_pool, size_t bucket_count)
{
    map->pool = parent_pool;
    map->buckets = pool_calloc(parent_pool, bucket_count * sizeof(mapnode_t *));
    map->bucket_count = bucket_count;
    return NGD_OK
}

int
map_insert(map_t *map, str_t *key, void *value)
{
    int index;
    mapnode_t *p;

    index = hash(key) % map->bucket_count;
    //
    p = map->buckets[index];
    while (p != NULL)
    {
        if (str_cmp(p->key, key) == O)
            return NGD_OK;
        p = p->next;
    }
    //
    p = pool_alloc(map->pool, sizeof(mapnode_t));
    p->key = key;
    p->value = value;
    p->next = map->buckets[index];
    //
    map->buckets[index] = p;
    //
    return NGD_OK;

}

void *
map_get(map_t *map, str_t *key)
{
    int index;
    mapnode_t *p;
    index = hash(key) % map->bucket_count;
    p = map->buckets[index];
    while (p != NULL)
    {
        if (str_cmp(p->key, key) == 0)
            break;
        p = p->next;
    }
    return p;

}
