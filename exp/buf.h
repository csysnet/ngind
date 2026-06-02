#ifndef BUF_H
#define BUF_H
//ben ngoai chi thay mot day cac byte nam thang tren mem,
//muon phan nao thi get() phan do
#define MLIMIT 16384

typedef struct {
    char buf[MLIMIT]; int tempfd;

    size_t buflimit; size_t used;
} buf_t;

typedef struct {
    void *p;
    size len;
} subbuf_t

typedef struct {

} block_t

//get subset
void *
buf_get(buf_t *buf, size_t start, size_t offset)
{

}

int
buf_enq(buf_t *buf, void *data, void *datalen)
{
    if (buf->used + new->len > buf->len)
        return -1;

    if

    ssize_t n_read;
    while (1)
    {
        n_read = read(conn->fd, (conn->inbuf).)
    }
}

int
buf_deq()


//donlen
void
buf_lshift(buf_t *buf, )




#end
