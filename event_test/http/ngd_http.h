//one http struct, please
#ifndef NGD_HTTP_CONN_H
#define NGD_HTTP_CONN_H
//limits
#define NGD_HTTP_LIMIT_REQLINE (1024 * 8) //8kb
#define NGD_HTTP_LIMIT_HEADER_LINE (1024 * 8) //8kb
#define NGD_HTTP_LIMIT_HEADERS (1024 * 32) //16kb
#define NGD_HTTP_LIMIT_BODY (1024 * 1024) //1mb
//contansts
#define NGD_HTTP_INBUF_SMALL 1024 //1kb
#define NGD_HTTP_INBUF_MEDIUM (1024 * 8) //8kb
#define NGD_HTTP_INBUF_LARGE (1024 * 16) //16kb
//
#define NGD_HTTP_FULL_HEADER_DONE 1
#define NGD_HTTP_FULL_CHUNK_DONE 2
#define NGD_HTTP_INVALID_REQUEST 3
//HTTP CODE
#define NGD_HTTP_OK 200
#define NGD_HTTP_BAD_REQUEST 400
//
//
typedef struct ngd_http_t ngd_http_t;
typedef struct ngd_http_header_t ngd_http_header_t;
// struct
struct ngd_http_header_t {
    str_t key;
    str_t value;
};
struct ngd_http_t {
    int state;
    ngd_pool_t *pool;
    //
    ngd_buf_t *inbuf;
    size_t recved_each;
    ngd_buf_t *outbuf;
    //parse stuff
    int state_parse;
    //request line
    str_t *smethod;
    str_t *suri;
    str_t *sver;
    u_char *method_start;
    u_char *method_end;
    u_char *uri_start;
    u_char *uri_end;
    u_char *ver_start;
    u_char *ver_end;
    //header

    ngd_list_t headers;
    u_char *key_start;
    u_char *key_end;
    str_t *mimes;
    u_char *value_start;
    u_char *value_end;
    size_t content_length;
    bool on_zlib;
    bool on_keep_alive;
    //body
    bool on_chunk;
    size_t chunk_size;
    size_t chunk_recved;
    u_char *chunk_start;
    u_char *chunk_end;
};
// connection
void ngd_http_init_conn(ngd_conn_t *c);
int ngd_http_close_conn(ngd_conn_t *c);
int ngd_http_handle_conn(ngd_conn_t *c);
int ngd_http_handle_conn_read(ngd_conn_t *c);
int ngd_http_handle_conn_write(ngd_conn_t *c);
int ngd_http_read_request(ngd_http_t *c);
// http
int ngd_http_parse_reqline(ngd_http_t *http); //(until pos == last) -> undone -> again
int ngd_http_parse_headers(ngd_http_t *http); //(pos <= last) -> done -> ok
int ngd_http_parse_body(ngd_http_t *http);
int ngd_http_build_resp(ngd_http_t *http);
int ngd_http_compress_resp(ngd_http_t *http);
int ngd_http_send_resp(ngd_http_t *http);
//
#endif


we only read more when
case 1:
    pos == last;
    case 1.1:
        if last == end;
            alloc larger,
case 2:
    break;

case 3:
    bruh;




if (request_recv > max_request_limit) {
    close
} else {
    if (reqline_recv > max_reqline_limit) {
        close
    }
    if (headers_recv > max_headers_limit) {
        close
    } else {
        if (header_limit_recv > max_header_line_limit) {
            close
        } else ..
    }

    if (body_recv > max_body_limit) {
         close
    }
}
i
case 1:


recv()
