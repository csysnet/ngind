#include "pool.h"
#include "stdlib.h"

static void *
pool_create_block(pool_t *pool)
{
    pool_block_t *block;

    block = (pool_block_t *)malloc(pool->bsize);

    block->fail = 0;
    block->used = sizeof(pool_block_t);
    block->last = sizeof(pool_block_t);
    block->end = block + pool->bsize;
    block->next = NULL;

    return block;
}

static void *
pool_alloc_block(pool_t *pool, size_t size)
{
    0,1,2,3
        last = 2
            end = 4
            size = 2;

    //walk through blocks,
    // if size fit, return p
    // else fail++, if fail > 4, tobusylist
    void *p;

    pool_block_t *prev_blk, *cur_blk;

    prev_blk = pool->blocks;
    cur_blk = pool->blocks;
    while (cur_blk != NULL)
    {
        if (size <= end - last) {
            p = last;
            last += size;
            return p;
        }
        cur_blk->fail++
        if (cur_blk->fail > 4) {
            prev_blk->next = cur_blk->next;
            if (pool->busy_blocks == NULL)
                cur_blk->next = NULL;
            else
                cur_blk->next = pool->busy_blocks;
            pool->busy_blocks = cur_blk;
        }

        prev_blk = cur_blk;
        cur_blk = cur_blk->next;
    }
    if ()

    if (pool->current == NULL) {
        pool->blocks = block;
        pool->current = block;
    }
    block = (pool_block_t *)malloc(pool->max);
    block->used = sizeof(pool_block_t);
    block->last = sizeof(pool_block_t);
    block->end = pool->max;
    block->next = NULL;
    pool->blocks = block;
    pool->current = block;

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
