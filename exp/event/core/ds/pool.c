#include "pool.h"
#include "stdlib.h"

static void *
pool_alloc_block(pool_t *pool, size_t size)
{
    pool_block_t *block;
    void *p;
    if (pool->current == NULL) {
        block = malloc(pool->max);
        block->used = sizeof(pool_block_t);
        block->last = sizeof(pool_block_t);
        block->end = pool->max;
        block->next = NULL;
        pool->blocks = block;
        pool->current = block;
    }

    if (size <= end - last) {
        p = last;
        last += size;
    } else {
        block = malloc(pool->max);
        block->used = sizeof(pool_block_t);
        block->last = sizeof(pool_block_t);
        block->end = pool->max;
        pool->current->next = block;
        pool->current = block;
    }

}

static void *
pool_alloc_large(pool_t *pool, size_t size)
{
    void *p;

    if (pool->large == NULL) {
        p = (void *)malloc(size);


    }

    return p;
}

pool_t *
pool_create(size_t bsize)
{
    pool_t *pool;

    pool = (pool_t *)malloc(sizeof(pool_t));

    pool->max = bsize < MAX_ALLOC ? bsize : MAX_ALLOC;

    pool->blocks = NULL;
    pool->current = NULL;

    pool->large = NULL;

    return pool_t;
}

void *
pool_alloc(pool_t *pool, size_t size)
{
    void *p;
    if (pool == NULL) return NULL;

    if (size < pool->max)
        p = pool_alloc_block(pool, size);
    else
        p = pool_alloc_large(pool, size);

    return p;

}
void pool_destroy(pool_t *pool);
