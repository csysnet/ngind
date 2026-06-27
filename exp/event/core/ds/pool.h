#ifndef POOL_H
#define POOL_H

#define POOL_ALIGNMENT       sizeof(unsigned long)
#define MAX_ALLOC  (4095)
#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)

typedef unsigned char u_char;

typedef struct pool_block_t {
    size_t used;
    u_char *last;
    u_char *end;
    struct pool_block_t *next;
} pool_block_t;

typedef struct pool_large_t {
    void *pdata;
    struct pool_large_t *next;
} pool_large_t;

typedef struct pool_t {
    size_t bmax;

    pool_block_t *blocks;
    pool_block_t *current;

    pool_large_t *large;

} pool_t;

pool_t *pool_create(size_t bsize);
void *pool_alloc(pool_t *pool, size_t size);
void pool_destroy(pool_t *pool);

#endif
