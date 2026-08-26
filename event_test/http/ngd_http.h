//one http struct, please
#ifndef NGD_HTTP_CONN_H
#define NGD_HTTP_CONN_H
//contansts
#define NGD_HTTP_INBUF_SMALL 1024
#define NGD_HTTP_INBUF_MEDIUM 8192
#define NGD_HTTP_INBUF_LARGE 16384
//
#define NGD_HTTP_BUCKET_COUNT 10
#define NGD_HTTP_FULL_HEADER_DONE 1
#define NGD_HTTP_FULL_CHUNK_DONE 2
#define NGD_HTTP_INVALID
//HTTP CODE
#define NGD_HTTP_OK 200
#define NGD_HTTP_BAD_REQUEST 400
//
//
typedef struct ngd_http_t ngd_http_t;
// struct
struct ngd_http_t {
    int state;
    int state_parse;
    ngd_pool_t *pool;
    ngd_conn_t *conn;
    //
    ngd_buf_t *inbuf;
    ngd_buf_t *outbuf;
    ngd_map_t *headers;
    //parse stuff
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
    u_char *key_start;
    u_char *key_end;
    u_char *value_start;
    u_char *value_end;
    //body
    size_t content_length;
    size_t body_received;
    bool on_chunk;
    size_t chunk_size;
    u_char *chunk_start;
    u_char *chunk_end;
};
// connection
int ngd_http_init_conn(ngd_conn_t *c);
int ngd_http_handle_conn(ngd_conn_t *c);
int ngd_http_close_conn(ngd_conn_t *c);
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
