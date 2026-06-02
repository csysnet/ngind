#ifndef POOL_H
#define POOL_H
//allo all here, no matter size
//one free

#define POOL_ALIGNMENT       sizeof(unsigned long)
#define NGX_MAX_ALLOC_FROM_POOL  (4095)
#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)
//
#define MAX_USED
#define BIG_THRESH
#define LARGE_THRESH

typedef unsigned char u_char;

typedef struct pool_block_t {
    u_char *last;
    u_char *end;
    struct pool_block_t *next;
} pool_data_t;

typedef struct pool_big_t {
    struct pool_big_t *next;
    void *alloc;
}

typedef struct pool_large_s {
    struct pool_large_t *next;
    void *alloc;
} pool_large_t;

typedef struct pool_s {
    pool_data_t d;

    size_t used;
    size_t max;

    struct pool_s *current;
    struct pool_large_s *large;
} pool_t;
b

pool_t *pool_create(size_t size);

void *pool_alloc(pool_t *pool, size_t size);
void *pool_realloc(pool_t *pool, size_t size);
void pool_destroy(pool_t *pool);

void *pool_alloc_block(pool_t *pool);
void *pool_allo_big(pool *pool);
void *pool_alloc_large(pool_t *pool);
#endif
