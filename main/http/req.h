#ifndef REQ_H
#define REQ_H

typedef struct str_t str_t;

typedef struct {
    str_t *smethod;
    str_t *suri;
    str_t *sver;

    buf_t *header_in;

}

#endif
