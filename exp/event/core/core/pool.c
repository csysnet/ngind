#include <stdlib.h>

#include "pool.h"

static void *
pool_create_block(pool_t *pool)
{
    pool_block_t *block;

    block = (pool_block_t *)malloc(BLOCKSIZE);

    block->fail = (int)0;
    block->used = (size_t)sizeof(pool_block_t);
    block->last = (u_char *)(block + sizeof(pool_block_t));
    block->end = (u_char *)(block + BLOCKSIZE);
    block->next = (pool_block_t *)NULL;

    return block;
}

static void *
pool_alloc_block(pool_t *pool, size_t size)
{
    //walk through blocks,
    // if size fit, return p
    // else fail++, if fail > 4, tobusylist
    void *p;

    pool_block_t *prev_blk, *cur_blk;

    prev_blk = pool->blocks;
    cur_blk = pool->blocks;
    while (cur_blk != NULL)
    {
        if (size <= cur_blk->end - cur_blk->last) {
            p = cur_blk->last;
            cur_blk->last += size;
            return p;
        }
        cur_blk->fail++;

        if (cur_blk->fail > 4) {
            prev_blk->next = cur_blk->next;
            if (pool->busy_blocks == NULL)
                cur_blk->next = NULL;
            else
                cur_blk->next = pool->busy_blocks;
            pool->busy_blocks = cur_blk;
            cur_blk = prev_blk->next;
            continue;
        }

        prev_blk = cur_blk;
        cur_blk = cur_blk->next;
    }

    cur_blk = pool_create_block(pool);
    if (pool->blocks == NULL)
        pool->blocks = cur_blk;
    else
        prev_blk->next = cur_blk;

    p = cur_blk->last;
    cur_blk->last += size;
    return p;
}

static void *
pool_alloc_large(pool_t *pool, size_t size)
{
    pool_large_t *l;
    void *p;

    l = (pool_large_t *)malloc(sizeof(pool_large_t) + size);
    p = l + sizeof(pool_large_t);
    if (pool->large_blocks == NULL)
        l->next = NULL;
    else
        l->next = pool->large_blocks;
    pool->large_blocks = l;

    return p;
}

pool_t *
pool_create()
{
    pool_t *pool;

    pool = (pool_t *)malloc(sizeof(pool_t));

    pool->blocks = NULL;
    pool->busy_blocks = NULL;

    pool->large_blocks = NULL;

    return pool;
}

void *
pool_alloc(pool_t *pool, size_t size)
{
    void *p;

    if (pool == NULL) return NULL;

    if (size <= BLOCKSIZE - sizeof(pool_block_t))
        p = pool_alloc_block(pool, size);
    else
        p = pool_alloc_large(pool, size);

    return p;

}

void *
pool_calloc(pool_t *pool, size_t size)
{
    void *p;

    if (pool == NULL) return NULL;

    p = pool_alloc(pool, size);
    if (p == NULL) return NULL;

    for (size_t i=0; i<size; i++)
        *((u_char *)p + i) = 0;

    return p;
}


void
pool_destroy(pool_t *pool)
{

    pool_block_t *blk, *next_blk;
    pool_large_t *large_blk, *next_large_blk;

    if (pool == NULL) return;

    blk = pool->blocks;
    while (blk) { next_blk = blk->next; free(blk); blk = next_blk; }

    blk = pool->busy_blocks;
    while (blk) { next_blk = blk->next; free(blk); blk = next_blk; }

    large_blk = pool->large_blocks;
    while (large_blk) { next_large_blk = large_blk->next; free(large_blk); large_blk = next_large_blk; }

    free(pool);
}
