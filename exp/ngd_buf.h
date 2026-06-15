#ifndef NGD_BUF_H
#define NGD_BUF_H
typedef unsigned char u_char;

#include <sys/types.h>

typedef struct {
    u_char *pos;
    u_char *last;
    off_t *fpos;
    off_t *flast;

    unsigned isfile;
    int fd;
    u_char *start;
    u_char *end;
} ngd_buf_t;

#endif
