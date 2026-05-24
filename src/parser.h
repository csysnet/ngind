#ifndef PARSER_H
#define PARSER_H

typedef struct {
    char *method;
    char *target;
    char *version;
} req_line;

typedef struct {
    char *key;
    char *value;
} header;

typedef struct {
    header *headers;
    size_t *hlen;
} req_header;

typedef struct {
    char *s;
} req_body;

typedef struct {
    req_line line;
    req_header headers;
    req_body body;
} req;


parse()

#endif
