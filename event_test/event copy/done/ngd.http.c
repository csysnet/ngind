#include "ngd_http.h"

#include <fcntl.h>
#include <unistd.h>
#include <string.h>


enum {
    NGD_HTTP_START = 0,
    NGD_HTTP_REQ_LINE,
    NGD_HTTP_HEADERS,
    NGD_HTTP_BODY,
    NGD_HTTP_BODY_CHUNK,
    NGD_HTTP_BODY_LEN,
    NGD_HTTP_BUILD_RESP,
    NGD_HTTP_SEND_RESP
};


void
ngd_http_init_conn(ngd_conn_t *c)
{
    ngd_pool_t *pool;
    ngd_http_t *http;
    void *p;

    pool = ngd_pool_create();
    if (pool == NULL) {
        ngd_conn_close(c);
        return;
    }

    http = ngd_pool_alloc(pool, sizeof(*http));
    if (http == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }

    memset(http, 0, sizeof(*http));

    p = ngd_pool_alloc(pool, NGD_HTTP_INBUF_SIZE);
    if (p == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }

    http->c = c;
    http->pool = pool;
    http->state = NGD_HTTP_START;
    http->state_parse = NGD_STATE_START;
    http->fd_temp = -1;

    ngd_buf_init(&http->inbuf,
                 p,
                 NGD_HTTP_INBUF_SIZE);

    ngd_conn_init(c,
                  ngd_http_handle_conn,
                  http,
                  60000);
}


void
ngd_http_handle_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    ngd_buf_t *b;
    ngd_http_header_t *header;
    ngd_list_node_t *node;

    ssize_t n;
    size_t len;
    size_t remaining;
    size_t available;
    n =
    int ret;
    void *p;

    http = ngd_conn_get_data(c);
    b = http->inbuf;

    /*
     * ------------------------------------------------------------
     * Timeout
     * ------------------------------------------------------------
     */

    if (ngd_conn_on_timeout(c)) {
        ngd_http_close_conn(http);
        return;
    }


    /*
     * ------------------------------------------------------------
     * Main state machine
     * ------------------------------------------------------------
     */

    for (;;) {

        /*
         * --------------------------------------------------------
         * START
         * --------------------------------------------------------
         */

        if (http->state == NGD_HTTP_START) {

            /*
             * Do not clear b->pos/b->last.
             *
             * They may contain the next keep-alive request.
             */

            http->state_parse = NGD_STATE_START;

            http->smethod.data = NULL;
            http->smethod.len = 0;
            http->suri.data = NULL;
            http->suri.len = 0;
            http->sver.data = NULL;
            http->sver.len = 0;

            http->content_length = 0;
            http->body_received = 0;

            http->chunk_size = 0;
            http->chunk_data_recved = 0;

            http->on_chunked = false;
            http->on_body_file = false;
            http->on_keep_alive = false;
            http->on_gzip = false;

            if (http->fd_temp >= 0) {
                close(http->fd_temp);
                http->fd_temp = -1;
            }

            ngd_list_reset(http->headers);

            http->state = NGD_HTTP_REQ_LINE;
            continue;
        }


        /*
         * --------------------------------------------------------
         * REQUEST LINE
         * --------------------------------------------------------
         */

        if (http->state == NGD_HTTP_REQ_LINE) {

            ret = ngd_http_parse_reqline(http);

            if (ret == NGD_OK) {

                http->smethod.data = http->method_start;
                http->smethod.len =
                    http->method_end - http->method_start;

                http->suri.data = http->uri_start;
                http->suri.len =
                    http->uri_end - http->uri_start;

                http->sver.data = http->ver_start;
                http->sver.len =
                    http->ver_end - http->ver_start;

                http->state_parse = NGD_STATE_START;
                http->state = NGD_HTTP_HEADERS;

                continue;
            }

            if (ret != NGD_AGAIN) {
                ngd_http_bad_request(http);
                return;
            }

            /*
             * Request line did not finish.
             *
             * If 16 KB is full, it is too large.
             */
            if (b->last == b->end) {
                ngd_http_bad_request(http);
                return;
            }

            if (!ngd_conn_on_read(c))
                return;

            ret = ngd_conn_recv(c,
                                b->last,
                                b->end - b->last,
                                &n);

            if (ret == NGD_OK) {
                b->last += n;
                continue;
            }

            if (ret == NGD_AGAIN)
                return;

            ngd_http_bad_request(http);
            return;
        }


        /*
         * --------------------------------------------------------
         * HEADERS
         * --------------------------------------------------------
         */

        if (http->state == NGD_HTTP_HEADERS) {

            ret = ngd_http_parse_headers(http);

            if (ret == NGD_OK) {

                header = ngd_pool_alloc(http->pool,
                                        sizeof(*header));

                if (header == NULL) {
                    ngd_http_bad_request(http);
                    return;
                }

                header->key.data = http->key_start;
                header->key.len =
                    http->key_end - http->key_start;

                header->value.data = http->value_start;
                header->value.len =
                    http->value_end - http->value_start;

                ngd_list_append(http->headers, header);

                continue;
            }

            if (ret == NGD_HTTP_FULL_HEADER_DONE) {

                /*
                 * Process all headers.
                 */
                for (node = http->headers->head;
                     node != NULL;
                     node = node->next) {

                    header = node->data;

                    if (ngd_str_iequal(
                            header->key,
                            NGD_STR_C("Content-Length"))) {

                        if (ngd_str_to_size(
                                header->value,
                                &http->content_length) != NGD_OK) {

                            ngd_http_bad_request(http);
                            return;
                        }
                    }

                    else if (ngd_str_iequal(
                                 header->key,
                                 NGD_STR_C("Transfer-Encoding"))) {

                        if (ngd_str_isin(
                                NGD_STR_C("chunked"),
                                header->value,
                                false)) {

                            http->on_chunked = true;
                        }
                    }

                    else if (ngd_str_iequal(
                                 header->key,
                                 NGD_STR_C("Connection"))) {

                        if (ngd_str_isin(
                                NGD_STR_C("keep-alive"),
                                header->value,
                                false)) {

                            http->on_keep_alive = true;
                        }
                    }

                    else if (ngd_str_iequal(
                                 header->key,
                                 NGD_STR_C("Accept-Encoding"))) {

                        if (ngd_str_isin(
                                NGD_STR_C("gzip"),
                                header->value,
                                false)) {

                            http->on_gzip = true;
                        }
                    }
                }

                /*
                 * Chunked body takes precedence.
                 */
                if (http->on_chunked) {
                    http->content_length = 0;
                }

                http->state = NGD_HTTP_BODY;
                continue;
            }

            if (ret != NGD_AGAIN) {
                ngd_http_bad_request(http);
                return;
            }

            /*
             * Headers must fit in the fixed 16-KB buffer.
             */
            if (b->last == b->end) {
                ngd_http_bad_request(http);
                return;
            }

            if (!ngd_conn_on_read(c))
                return;

            ret = ngd_conn_recv(c,
                                b->last,
                                b->end - b->last,
                                &n);

            if (ret == NGD_OK) {
                b->last += n;
                continue;
            }

            if (ret == NGD_AGAIN)
                return;

            ngd_http_bad_request(http);
            return;
        }


        /*
         * --------------------------------------------------------
         * BODY
         * --------------------------------------------------------
         */

        if (http->state == NGD_HTTP_BODY) {

            if (http->on_chunked) {

                http->state_parse = NGD_HTTP_CHUNK_SIZE;
                http->state = NGD_HTTP_BODY_CHUNK;

                continue;
            }

            if (http->content_length == 0) {
                http->state = NGD_HTTP_BUILD_RESP;
                continue;
            }

            /*
             * Body is the only thing with an explicit size limit.
             */
            if (http->content_length > NGD_HTTP_LIMIT_BODY) {
                ngd_http_bad_request(http);
                return;
            }

            if (http->content_length <= NGD_HTTP_INBUF_SIZE) {

                /*
                 * Body lives in the input buffer.
                 *
                 * Existing bytes may already contain part/all
                 * of the body.
                 */
                http->state = NGD_HTTP_BODY_LEN;
                continue;
            }

            /*
             * Large body -> temporary file.
             */
            http->fd_temp = ngd_http_create_temp_file();

            if (http->fd_temp < 0) {
                ngd_http_bad_request(http);
                return;
            }

            http->on_body_file = true;
            http->offset = 0;

            http->state = NGD_HTTP_BODY_LEN;
            continue;
        }


        /*
         * --------------------------------------------------------
         * CONTENT-LENGTH BODY
         * --------------------------------------------------------
         */

        if (http->state == NGD_HTTP_BODY_LEN) {

            remaining =
                http->content_length -
                http->body_received;

            /*
             * Consume bytes already in the input buffer.
             */
            available = b->last - b->pos;

            if (available != 0) {

                len = available;

                if (len > remaining)
                    len = remaining;

                if (http->on_body_file) {

                    if (ngd_write_all(http->fd_temp,
                                      b->pos,
                                      len) != NGD_OK) {

                        ngd_http_bad_request(http);
                        return;
                    }

                }

                /*
                 * For memory body, the bytes remain in the
                 * input buffer and can be referenced by the
                 * request.
                 */
                b->pos += len;
                http->body_received += len;

                if (http->body_received ==
                    http->content_length) {

                    http->state = NGD_HTTP_BUILD_RESP;
                    continue;
                }

                continue;
            }

            /*
             * Buffer is empty.
             */
            b->pos = b->start;
            b->last = b->start;

            if (!ngd_conn_on_read(c))
                return;

            ret = ngd_conn_recv(c,
                                b->last,
                                b->end - b->last,
                                &n);

            if (ret == NGD_OK) {
                b->last += n;
                continue;
            }

            if (ret == NGD_AGAIN)
                return;

            ngd_http_bad_request(http);
            return;
        }


        /*
         * --------------------------------------------------------
         * CHUNKED BODY
         * --------------------------------------------------------
         *
         * ngd_http_parse_chunk() maintains its own parser state.
         *
         * A chunk can be larger than 16 KB.
         *
         * If the chunk/body becomes large, switch to temp file.
         * --------------------------------------------------------
         */

        if (http->state == NGD_HTTP_BODY_CHUNK) {

            ret = ngd_http_parse_chunk(http);

            if (ret == NGD_DONE) {

                http->state = NGD_HTTP_BUILD_RESP;
                continue;
            }

            if (ret == NGD_ERR) {
                ngd_http_bad_request(http);
                return;
            }

            /*
             * Parser needs more data.
             *
             * If buffer is full, consume body data or,
             * if necessary, move to temp file.
             */
            if (b->last == b->end) {

                len = b->last - b->pos;

                if (len != 0) {

                    if (!http->on_body_file &&
                        http->body_received + len >
                        NGD_HTTP_INBUF_SIZE) {

                        http->fd_temp =
                            ngd_http_create_temp_file();

                        if (http->fd_temp < 0) {
                            ngd_http_bad_request(http);
                            return;
                        }

                        http->on_body_file = true;
                        http->offset = 0;

                        /*
                         * Move everything currently buffered
                         * into the temporary file.
                         */
                        if (ngd_write_all(
                                http->fd_temp,
                                b->pos,
                                len) != NGD_OK) {

                            ngd_http_bad_request(http);
                            return;
                        }

                        http->body_received += len;

                        b->pos = b->start;
                        b->last = b->start;
                    }

                    else if (http->on_body_file) {

                        if (ngd_write_all(
                                http->fd_temp,
                                b->pos,
                                len) != NGD_OK) {

                            ngd_http_bad_request(http);
                            return;
                        }

                        http->body_received += len;

                        b->pos = b->start;
                        b->last = b->start;
                    }
                }

                /*
                 * If it is still not a file body and the parser
                 * needs more room, the chunked body has exceeded
                 * the memory window.
                 */
                if (!http->on_body_file &&
                    b->last == b->end) {

                    ngd_http_bad_request(http);
                    return;
                }
            }

            if (!ngd_conn_on_read(c))
                return;

            ret = ngd_conn_recv(c,
                                b->last,
                                b->end - b->last,
                                &n);

            if (ret == NGD_OK) {
                b->last += n;
                continue;
            }

            if (ret == NGD_AGAIN)
                return;

            ngd_http_bad_request(http);
            return;
        }


        /*
         * --------------------------------------------------------
         * BUILD RESPONSE
         * --------------------------------------------------------
         */

        if (http->state == NGD_HTTP_BUILD_RESP) {

            ret = ngd_http_build_resp(http);

            if (ret != NGD_OK) {
                ngd_http_bad_request(http);
                return;
            }

            http->state = NGD_HTTP_SEND_RESP;
            continue;
        }


        /*
         * --------------------------------------------------------
         * SEND RESPONSE
         * --------------------------------------------------------
         *
         * ngd_http_send_resp() does:
         *
         *     source response part
         *          ↓
         *       gzip part
         *          ↓
         *       send part
         *
         * It does NOT compress the entire response first.
         * --------------------------------------------------------
         */

        if (http->state == NGD_HTTP_SEND_RESP) {

            if (!ngd_conn_on_write(c))
                return;

            ret = ngd_http_send_resp(http);

            if (ret == NGD_AGAIN)
                return;

            if (ret == NGD_ERR) {
                ngd_http_bad_request(http);
                return;
            }

            if (ret == NGD_DONE) {

                if (http->fd_temp >= 0) {
                    close(http->fd_temp);
                    http->fd_temp = -1;
                }

                if (http->on_keep_alive) {

                    /*
                     * Important:
                     *
                     * DO NOT reset inbuf.
                     *
                     * b->pos ... b->last may contain the
                     * beginning of the next request.
                     */
                    http->state = NGD_HTTP_START;
                    continue;
                }

                ngd_http_close_conn(http);
                return;
            }

            continue;
        }

        ngd_http_bad_request(http);
        return;
    }
}
