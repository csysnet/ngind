#ifndef BUF_H
#define BUF_H

#include "core.h"

typedef struct buf_t {
    u_char *pos;
    u_char *last;
    off_t *fpos;
    off_t *flast;
    int fd;
    unsigned file:1;
} buf_t;

#endif
