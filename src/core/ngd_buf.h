#ifndef NGD_BUF_H
#define NGD_BUF_H
typedef unsigned char u_char;

#include <sys/types.h>

typedef struct {
    u_char *pos;
    u_char *last;
    off_t *fpos;
    off_t *flast;
} ngd_buf_t;

inline void
ngd_buf_init(ngd_buf_t *b, void *data, size_t len)
{
    b->start = data;
    b->pos = b->start;
    b->last = b->start;
    b->end = b->start + len;
}
#endif
