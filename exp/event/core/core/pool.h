#ifndef POOL_H
#define POOL_H
#include <stddef.h>
#include "types.h"

#define POOL_ALIGNMENT       sizeof(unsigned long)
#define BLOCKSIZE  4096


typedef struct pool_block_t {
    int fail;
    size_t used;
    u_char *last;
    u_char *end;
    struct pool_block_t *next;
} pool_block_t;

typedef struct pool_large_t {
    struct pool_large_t *next;
} pool_large_t;

typedef struct pool_t {
    pool_block_t *blocks;
    pool_block_t *busy_blocks;

    pool_large_t *large_blocks;

} pool_t;

pool_t *pool_create();
void *pool_alloc(pool_t *pool, size_t size);
void *pool_calloc(pool_t *pool, size_t size);
void pool_destroy(pool_t *pool);

#endif
