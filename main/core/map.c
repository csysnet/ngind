#include "map.h"
#include "pool.h"
#include "str.h"

static size_t
hash(str_t *key, size_t m)
{

    int hash_val;
    //
    hash_val = 0;
    //
    for (size_t i=0; i < key->len; i++)
    {
        hash_val = (hash_val * key->p[i] + 1) % m;
    }
    // printf("reach hash: %lu\n",hash_val);
    //
    return hash_val;
}

int
map_init(map_t *map, pool_t *parent_pool)
{
    map->pool = parent_pool;
    map->buckets = pool_calloc(parent_pool, MAP_BUCKET_COUNT * sizeof(mapnode_t *));
    map->bucket_count = MAP_BUCKET_COUNT;
    return NGD_OK;
}

int
map_insert(map_t *map, str_t *key, void *value)
{
    int index;
    mapnode_t *p;
    index = hash(key, map->bucket_count);
    //
    p = map->buckets[index];
    while (p != NULL)
    {
        if (str_cmp(p->key, key) == 0)
            return NGD_OK;
        p = p->next;
    }
    //
    p = pool_alloc(map->pool, sizeof(mapnode_t));
    p->key = key;
    p->value = value;
    p->next = map->buckets[index];
    map->buckets[index] = p;
    //
    return NGD_OK;

}

void *
map_get(map_t *map, str_t *key)
{
    int index;
    mapnode_t *p;

    index = hash(key, map->bucket_count);
    p = map->buckets[index];
    while (p != NULL)
    {
        if (str_cmp(p->key, key) == 0) {
            return p->value;
        }
        p = p->next;
    }
    return NULL;

}
