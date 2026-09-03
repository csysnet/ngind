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
} buf_t;

#endif
