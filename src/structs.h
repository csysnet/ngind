#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    void *s;
    size_t slen;
} str;


typedef struct {
    str method;
    str url;
    str ver;
} req_line;

typedef struct {
    str key;
    str value;
    header *next;
} header;

typedef struct {
    header *header;
} req_header;

typedef struct {
    str payload;
} req_body;

typedef struct {
    req_line line;
    req_header headers;
    req_body body;
} req;


#endif
