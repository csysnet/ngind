#ifndef BUF_H
#define BUF_H
//
#include "core.h"
//
typedef struct buf_t {
    u_char *start;
    u_char *pos;
    u_char *last;
    u_char *end;
    //
    int fd;
    off_t fpos;
    off_t flast;
    unsigned file:1;
} buf_t;

inline void
ngd_buf_init(ngd_buf_t *b, void *data, size_t len)
{
    b->start = b->start;
    b->pos = b->start;
    b->last = b->start;
    b->end = (u_char *)b->data + len;
}


#endif
