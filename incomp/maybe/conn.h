#ifndef CONN_H
#define CONN_H
//user use existed buffer, but assum using offset
//handle read write
//generic, mot cai ham viet san, public cach dieu khien no
//infinite, for each avai, greeding
// pov
// send til mem full, upmore
//khi tra ve pointer, thang nguoi dung muon lam gi tren do thi lam, tao khong quan tam
typedef void (*onread_t)(conn_t c);
typedef void (*onwrite_t)(conn_t c);
typedef struct {
    int fd;

    buf_t in;
    buf_t out;

    void *proto_cxt;
    onread_t onread;
    onwrite_t onwrite;

} conn_t;


void//just tell me where read, u should assume it continuous
conn_read(conn_t conn, size_t start, size_t end)
{

}

conn_t *
conn_create(size_t inlen, size_t outlen, onread_t onread, onwrite_t onwrite)
{
    conn_t *c = (conn_t *)malloc(sizeof(conn_t));
        ..init
    return c;
}

void
conn_close(conn_t *c)
{
    ...free field
    free(c)
}

void
conn_buf_append()
{

}

void
conn_handle(conn_t *c)
{
    ssize_t n_read;
    int n_ready;

    for(;;)
    {
        n_ready = epoll_wait();
        for (int i = 0; i < n_ready; i++)
        {
            if (ev[i] is accept)


        }
    }
}

void
conn_recv(conn_t *c, )
{

}

void
conn_send(conn_t *c, )

#endif
