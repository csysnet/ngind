#include "buf.h"


int
buf_init(buf_t *b, size_t size)
{
    b->start = b;
    //
    b->pos = b->start;
    b->last = b->start;
    b->end = b->start + n;
    //
    return NGD_OK
}
