#include <stdlib.h>
//
#include "ngd_pool.h"
//
typedef struct ngd_pool_block_t ngd_pool_block_t;
typedef struct ngd_pool_large_t ngd_pool_large_t;
//
struct ngd_pool_block_t {
    u_char *last;
    u_char *end;
    ngd_pool_block_t *next;
};
//
struct ngd_pool_large_t {
    ngd_pool_large_t *next;
};
//
struct ngd_pool_t {
    ngd_pool_block_t *blocks;
    ngd_pool_large_t *large_blocks;
    size_t block_range;
};
// helpers
static ngd_pool_block_t *
create_block(void)
{
    ngd_pool_block_t *blk;
    //
    blk = (ngd_pool_block_t *)malloc(NGD_POOL_BLOCKSIZE);
    if (blk == NULL)
        return NULL;
    //
    blk->last = (u_char *)blk + sizeof(ngd_pool_block_t);
    blk->end = (u_char *)blk + NGD_POOL_BLOCKSIZE;
    blk->next = NULL;
    //
    return blk;
}
//
static void *
alloc_block(ngd_pool_t *pool, size_t size)
{
    ngd_pool_block_t *blk;
    ngd_pool_block_t *new_blk;
    void *p;
    //
    for (blk = pool->blocks; blk != NULL; blk = blk->next)
    {
        if (blk->end - blk->last >= size) {
            p = blk->last;
            blk->last += size;

            return p;
        }
    }

    new_blk = create_block();
    if (new_blk == NULL)
        return NULL;

    new_blk->next = pool->blocks;
    pool->blocks = new_blk;

    p = new_blk->last;
    new_blk->last += size;

    return p;
}
//
static void *
alloc_large(ngd_pool_t *pool, size_t size)
{
    ngd_pool_large_t *large_blk;
    void *p;
    //
    large_blk = (ngd_pool_large_t *)malloc(sizeof(ngd_pool_large_t) + size);
    if (pool->large_blocks == NULL)
        large_blk->next = NULL;
    else
        large_blk->next = pool->large_blocks;
    pool->large_blocks = large_blk;
    //
    p = (u_char *)large_blk + sizeof(ngd_pool_large_t);
    //
    return p;
}
//
ngd_pool_t *
ngd_pool_create()
{
    ngd_pool_t *pool;
    //
    pool = (ngd_pool_t *)malloc(sizeof(ngd_pool_t));
    if (pool == NULL)
        return NULL;
    //
    pool->blocks = NULL;
    pool->large_blocks = NULL;
    pool->block_range = NGD_POOL_BLOCKSIZE - sizeof(ngd_pool_block_t);
    //
    return pool;
}
//
void *
ngd_pool_alloc(ngd_pool_t *pool, size_t size)
{
    void *p;
    //
    if (size <= pool->block_range)
        p = alloc_block(pool, size);
    else
        p = alloc_large(pool, size);
    //
    return p;
}
//
void
ngd_pool_destroy(ngd_pool_t *pool)
{
    ngd_pool_block_t *blk, *next_blk;
    ngd_pool_large_t *large_blk, *next_large_blk;
    //
    blk = pool->blocks;
    while(blk)
    {
        next_blk = blk->next;
        free(blk);
        blk = next_blk;
    }
    //
    large_blk = pool->large_blocks;
    while (large_blk)
    {
        next_large_blk = large_blk->next;
        free(large_blk);
        large_blk = next_large_blk;
    }
    //
    free(pool);
}
