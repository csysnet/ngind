#ifndef NGD_FILE_H
#define NGD_FILE_H
//
#include "ngd_core.h"
//
typedef struct ngd_file_t ngd_file_t;
//
struct ngd_file_t {
    int fd;
};
//
#define NGD_FILE_DONE 1
//
void ngd_file_init(ngd_file_t *f);
void ngd_file_close(ngd_file_t *f);
int ngd_file_open(ngd_file_t *f, const char *path);
int ngd_file_read(ngd_file_t *f, u_char *buf, size_t len, size_t *bytes_read);
int ngd_file_get_size(ngd_file_t *f, size_t *size);
//
#endif
