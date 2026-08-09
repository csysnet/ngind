//
// #include <>
//
#include "req.h"
#include "buf.h"
#include "str.h"
#include "map.h"
#include "event.h"
#include "conn.h"
#include "pool.h"
//
static int counter;

int
http_wait_req(event_t *rev)
{
    printf("reach wait\n");
    conn_t *c;
    req_t *r;
    buf_t *b;
    ssize_t n;
    //
    c = rev->pdata;
    //
    b = pool_alloc(c->pool, sizeof(buf_t) + MAX_INBUF);
    fprintf(stderr, "pool: %p\n", c->pool->blocks);
    fprintf(stderr, "conn: %p\n", c);
    fprintf(stderr, "buf: %p\n", b);
    fprintf(stderr, RED"diff pool & conn: %p\n"RESET, (u_char *)c - (u_char *)b);
    fprintf(stderr, RED"diff conn & buf: %p\n"RESET, (u_char *)c - (u_char *)b);
    b->start = (u_char *)b + sizeof(buf_t);
    b->pos = b->start;
    b->last = b->start;
    b->end = b->start + sizeof(buf_t) + MAX_INBUF;
    //
    r = pool_alloc(c->pool, sizeof(req_t));
    r->header_in = b;
    r->headers = pool_alloc(c->pool, sizeof(map_t));
    map_init(r->headers, c->pool);
    r->smethod = pool_alloc(c->pool, sizeof(str_t));
    r->suri = pool_alloc(c->pool, sizeof(str_t));
    r->sver = pool_alloc(c->pool, sizeof(str_t));
    //
    c->pdata = r;
    //
    rev->handler = http_proc_reqline;
    http_proc_reqline(rev);
    //
    return NGD_OK;
}
int
http_empty_handler(event_t *wev)
{
    return NGD_OK;
}

int
http_block_reading(event_t *rev)
{
    return NGD_OK;
}

int
http_read_req(event_t *rev)
{
    fprintf(stderr, "count: "RED"%d\n"RESET, counter);
    fprintf(stderr, "fd: %d\n", ((conn_t *)rev->pdata)->fd);
    counter++;
    // if (counter > 4)
    //     sleep(10);
    conn_t *c;
    req_t *r;
    buf_t *b;
    ssize_t n;
    //
    c = rev->pdata;
    r = c->pdata;
    b = r->header_in;
    //
    n = b->last - b->pos;
    fprintf(stderr, "unread: %ld\n", n);
    fprintf(stderr, "freespace now: %ld\n", b->end - b->last);
    if (n > 0)
    // fprintf(stderr, "read shitfd: %d\n", ((conn_t *)rev->pdata)->fd);
        return n;
    b->pos = b->last;
    if (b->end - b->last == 0) return NGD_AGAIN;
    // printf("shitdiff: %lu\n", b->end - b->last);
    n = c->recv(c, b->last, b->end - b->last);
    fprintf(stderr, RED"n read: %ld\n"RESET, n);
    if (n < 0)
        return NGD_AGAIN;
    b->last += n;

    return n;
}



int
http_proc_reqline(event_t *rev)
{
    int rc;
    conn_t *c;
    req_t *r;
    ssize_t n;
    buf_t *b;
    //
    c = rev->pdata;
    r = c->pdata;
    b = r->header_in;
    counter = 0;
    //
    for (;;)
    {
    // fprintf(stderr, "startokokokkfd: %d\n", c->fd);
        n = http_read_req(rev);
    // fprintf(stderr, "endokokokkfd: %d\n", c->fd);
        rc = http_parse_reqline(r, r->header_in);

        if (rc == NGD_OK) {
            r->smethod->p = r->start_method;
            r->smethod->len = r->end_method - r->start_method;
            r->suri->p = r->start_uri;
            r->suri->len = r->end_uri - r->start_uri;
            r->sver->p = r->start_ver;
            r->sver->len = r->end_ver - r->start_ver;
            ps(r->smethod);
            printf(" ");
            ps(r->suri);
            printf(" ");
            ps(r->sver);
            printf("\n");
            http_proc_headers(rev);
            rev->handler = http_proc_headers;
            break;
        }

        if (rc == NGD_AGAIN)
            continue;
    }
    //
    return NGD_OK;
}

int
http_proc_headers(event_t *rev)
{
    fprintf(stderr, "reach proc headers\n");
    conn_t *c;
    req_t *r;
    buf_t *b;
    str_t *key, *value;
    ssize_t n;
    int rc;
    long body_len;
    //
    c = rev->pdata;
    r = c->pdata;
    b = r->header_in;
    fprintf(stderr, "fd: %d\n", c->fd);

    //
    // printf("diff: %lu\n", b->last - b->pos);
    // printf("reach proc header\n\n");
    for (;;)
    {
        n = http_read_req(rev);
        if (n == NGD_AGAIN)
            break;
        rc = http_parse_header_line(r, r->header_in);
        if (rc == NGD_OK) {
            key = pool_alloc(c->pool, sizeof(str_t));
            key->p = r->start_key;
            key->len = r->end_key - r->start_key;
            ps(key);
            //
            value = pool_alloc(c->pool, sizeof(str_t));
            value->p = r->start_value;
            value->len = r->end_value - r->start_value;
            // printf("valuelen: %lu\n", value->len);
            printf(": ");
            ps(value);
            printf("\n");
            //
            map_insert(r->headers, key, value);
            //
            continue;
        }

        if (rc == HTTP_PARSE_HEADER_DONE) {
            printf("each ok\n");
            key = pool_alloc(c->pool, sizeof(str_t));
            //
            str_from_chars(key, "Connection");
            value = map_get(r->headers, key);
            //
            str_from_chars(key, "Content-Length");
            value = map_get(r->headers, key);
            if (value) {
                body_len = str_to_long(c->pool, value);
                r->content_length = body_len;
                ps(key);
                printf(": %ld\n", body_len);
            }
            //
            str_from_chars(key, "Transfer-Encoding");
            if (value) {
                value = map_get(r->headers, key);
                r->chunked = 1;
            }
            fprintf(stderr, "bro\n");

            rev->handler = http_build_req;
            http_build_req(rev);
            // r->body_received = 0;
            // r->header_in->pos = r->header_in->start;
            // r->header_in->last = r->header_in->start;
            // rev->handler = http_proc_body;
            // http_proc_body(rev);
            break;
        }


        if (rc == NGD_AGAIN)
            continue;

    }
}
//
// int
// http_proc_body(event_t *rev)
// {   //guarantee content_length
//     conn_t *c;
//     req_t *r;
//     buf_t *b;
//     int ret;
//     ssize_t n;
//     //
//     c = rev->pdata;
//     r = c->pdata;
//     b = r->header_in;
//     //
//     for (;;)
//     {
//         n = http_read_req(rev);
//         // printf("n: %d\n", n);
//         // sleep(1000000);
//         if (n == NGD_AGAIN)
//             break;
//         ret = http_parse_body(r, b);
//         if (ret == NGD_OK) {
//             rev->handler = http_build_req;
//             http_build_req(rev);
//             printf("reach ok body\n");
//             break;
//         }

//         if (ret == NGD_AGAIN) {
//             continue;
//         }
//     }

//     return NGD_OK;
//
//
int
http_proc_body(event_t *rev)
{   //guarantee content_length
    fprintf(stderr, RED"reach proc body\n"RESET);
    conn_t *c;
    req_t *r;
    buf_t *b;
    int ret;
    ssize_t n;
    //
    c = rev->pdata;
    r = c->pdata;
    b = r->header_in;
    //
    for (;;)
    {
        fprintf(stderr, RED"con-len: %lu\n"RESET, r->content_length);
        n = http_read_req(rev);
        // printf("n: %d\n", n);
        // sleep(1000000);
        if (n == NGD_AGAIN)
        {
            if (b->end - b->last == 0) {
                fprintf(stderr, RED"reach reset when %lu\n"RESET, r->body_received);
                b->pos = b->start;
                b->last = b->start;
            } else {
                break;
            }
        }
        r->body_received += n;
        b->pos += n;
        fprintf(stderr, "is this step pos==last: %lu\n",b->last - b->pos);
        if (r->body_received == r->content_length) {
            fprintf(stderr, "reach ok body second\n");
            rev->handler = http_build_req;
            http_build_req(rev);
            break;
        }
        fprintf(stderr, RED"received: %lu\n"RESET, r->body_received);
    }

    return NGD_OK;
}
//
int
http_proc_switch(event_t *rev)
{
    // printf("%")
    return NGD_OK;
}

int
http_build_req(event_t *wev)
{
    fprintf(stderr, "reach build req\n");
    conn_t *c;
    req_t *r;
    ssize_t n;
    // u_char res[] =
    //     "HTTP/1.1 200 OK\r\n"
    //     "Content-Type: text/plain\r\n"
    //     "Content-Length: 13\r\n"
    //     "Connection: close\r\n"
    //     "\r\n"
    //     "Hello, world!";
    u_char res[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 16\r\n"
        "Connection: keep-alive\r\n"
        "\r\n"
        "Hello, world!dat";
    //
    c = wev->pdata;
    r = c->pdata;
    //
    n = c->send(c, res, sizeof(res) - 1);
    // fprintf(stderr, "fd: %d\n", c->fd);
    // n = c->send(c, res + sizeof(res) - 1 - 10, 10);
    // printf("nsend: %ld\n", n);
    conn_close(c);
    fprintf(stderr, RED"reach build well\n");
    perror(RED"send"RESET);
    // sleep(10);
    //
    fprintf(stderr, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    return NGD_OK;
}
// int
// http_build_req(event_t *wev)
// {
//     fprintf(stderr, "reach build req\n");
//     conn_t *c;
//     req_t *r;
//     ssize_t n;
//     // u_char res[] =
//     //     "HTTP/1.1 200 OK\r\n"
//     //     "Content-Type: text/plain\r\n"
//     //     "Content-Length: 13\r\n"
//     //     "Connection: close\r\n"
//     //     "\r\n"
//     //     "Hello, world!";
//     u_char res[] =
//         "HTTP/1.1 200 OK\r\n"
//         "Content-Type: text/plain\r\n"
//         "Content-Length: %lu\r\n"
//         "Connection: keep-alive\r\n"
//         "\r\n";
//     //
//     c = wev->pdata;
//     r = c->pdata;
//     //
//     n = c->send(c, res, sizeof(res) - 1);
//     // fprintf(stderr, "fd: %d\n", c->fd);
//     // n = c->send(c, res + sizeof(res) - 1 - 10, 10);
//     // printf("nsend: %ld\n", n);
//     conn_close(c);
//     perror(RED"send"RESET);
//     // sleep(10);
//     //
//     fprintf(stderr, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
//     return NGD_OK;
// }
