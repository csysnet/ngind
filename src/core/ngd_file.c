#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
//
#include "ngd_file.h"
//
void
ngd_file_init(ngd_file_t *f)
{
    f->fd = -1;
}
//
void
ngd_file_close(ngd_file_t *f)
{
    if (f->fd != -1) {
        close(f->fd);
        f->fd = -1;
    }
}
//
int
ngd_file_open(ngd_file_t *f, const char *path)
{
    f->fd = open(path, O_RDONLY);
    //
    if (f->fd == -1)
        return NGD_ERR;
    //
    return NGD_OK;
}


int
ngd_file_read(ngd_file_t *f, u_char *buf, size_t len,
    size_t *bytes_read)
{
    ssize_t n;
    //
    *bytes_read = 0;
    //
    n = read(f->fd, buf, len);
    //
    if (n == -1)
        return NGD_ERR;
    //
    if (n == 0)
        return NGD_FILE_DONE;
    //
    *bytes_read = (size_t) n;

    return NGD_OK;
}
//
int
ngd_file_get_size(ngd_file_t *f, size_t *size)
{
    struct stat st;
    //
    if (fstat(f->fd, &st) == -1)
        return NGD_ERR;
    //
    *size = (size_t) st.st_size;
    //
    return NGD_OK;
}
