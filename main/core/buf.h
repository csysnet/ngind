#ifndef BUF_H
#define BUF_H

#include "core.h"

typedef struct buf_t {
    u_char *pos;
    u_char *last;
    u_char *start;
    u_char *end;
    //
    int fd;
    off_t flast;
    off_t fpos;
    unsigned file:1;
} buf_t;


#endif
