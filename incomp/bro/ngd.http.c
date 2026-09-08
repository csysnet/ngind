#include "ngd_http.h"

#include <string.h>
#include <unistd.h>


void
ngd_http_init_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    ngd_pool_t *pool;

    /*
     * Your existing pool creation/allocation API.
     */
    pool = ngd_pool_create();

    if (pool == NULL) {
        ngd_conn_close(c);
        return;
    }

    http = ngd_pool_alloc(pool, sizeof(ngd_http_t));

    if (http == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }

    memset(http, 0, sizeof(ngd_http_t));

    http->pool = pool;

    /*
     * Persistent high-level state.
     *
     * The actual switch variable in handle_conn() remains local.
     */
    http->state = NGD_STATE_START;

    /*
     * Shared incremental component state.
     */
    http->state_req = NGD_STATE_START;

    http->fd_temp = -1;

    /*
     * Exactly fixed-size buffers.
     */
    http->inbuf = ngd_pool_alloc(
        pool,
        sizeof(ngd_buf_t)
    );

    if (http->inbuf == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }

    ngd_buf_init(
        http->inbuf,
        ngd_pool_alloc(pool, NGD_HTTP_INBUF_SIZE),
        NGD_HTTP_INBUF_SIZE
    );

    if (http->inbuf->start == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }


    http->bodybuf = ngd_pool_alloc(
        pool,
        sizeof(ngd_buf_t)
    );

    if (http->bodybuf == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }

    ngd_buf_init(
        http->bodybuf,
        ngd_pool_alloc(pool, NGD_HTTP_BODYBUF_SIZE),
        NGD_HTTP_BODYBUF_SIZE
    );

    if (http->bodybuf->start == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }


    http->outbuf = ngd_pool_alloc(
        pool,
        sizeof(ngd_buf_t)
    );

    if (http->outbuf == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }

    ngd_buf_init(
        http->outbuf,
        ngd_pool_alloc(pool, NGD_HTTP_OUTBUF_SIZE),
        NGD_HTTP_OUTBUF_SIZE
    );

    if (http->outbuf->start == NULL) {
        ngd_pool_destroy(pool);
        ngd_conn_close(c);
        return;
    }


    /*
     * Connection owns HTTP through pdata/data.
     */
    ngd_conn_init(
        c,
        ngd_http_handle_conn,
        http,
        60000
    );
}


int
ngd_http_close_conn(ngd_conn_t *c)
{
    ngd_http_t *http;

    http = ngd_conn_get_data(c);

    if (http != NULL) {

        if (http->fd_temp != -1) {

            close(http->fd_temp);
            http->fd_temp = -1;
        }

        /*
         * gzip cleanup should happen here if necessary.
         */

        if (http->pool != NULL) {
            ngd_pool_destroy(http->pool);
        }
    }

    ngd_conn_close(c);

    return NGD_OK;
}


static int
ngd_http_read_request(ngd_conn_t *c)
{
    ngd_http_t *http;
    ngd_buf_t *b;

    size_t n;
    size_t left;

    int ret;

    http = ngd_conn_get_data(c);
    b = http->inbuf;

    /*
     * Buffer invariant:
     *
     * [start, pos) consumed
     * [pos, last) unread
     * [last, end) free
     */


    /*
     * If there is no free space, compact unread data.
     */
    if (b->last == b->end) {

        if (b->pos != b->start) {

            left = b->last - b->pos;

            memmove(
                b->start,
                b->pos,
                left
            );

            b->pos = b->start;
            b->last = b->start + left;
        }
    }


    /*
     * Still full.
     *
     * For request line/headers this means they exceeded
     * the fixed input-buffer limit.
     *
     * Body processing must consume data before reaching
     * this situation.
     */
    if (b->last == b->end) {
        return NGD_ERR;
    }


    ret = ngd_conn_recv(
        c,
        b->last,
        b->end - b->last,
        &n
    );

    if (ret == NGD_OK) {

        if (n == 0) {
            return NGD_ERR;
        }

        b->last += n;

        return NGD_OK;
    }


    if (ret == NGD_AGAIN) {
        return NGD_AGAIN;
    }


    return NGD_ERR;
}


void
ngd_http_handle_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    ngd_buf_t *b;

    int ret;

    /*
     * High-level HTTP state machine.
     *
     * Local variable as required.
     */
    enum {
        ps_start = NGD_STATE_START,
        ps_reqline,
        ps_headers,
        ps_body,
        ps_build_resp,
        ps_send_resp,
        ps_close
    } state;


    http = ngd_conn_get_data(c);

    if (http == NULL) {
        ngd_conn_close(c);
        return;
    }


    b = http->inbuf;


    /*
     * Restore the phase from the previous invocation.
     *
     * state itself is local.
     */
    state = http->state;


    for (;;) {

        switch (state) {


        /*
         * =====================================================
         * START NEW REQUEST
         * =====================================================
         */
        case ps_start:

            /*
             * IMPORTANT:
             *
             * Do not reset inbuf->pos or inbuf->last.
             *
             * They may already contain the beginning of the
             * next pipelined/keep-alive request.
             */


            /*
             * New request parser starts fresh.
             */
            http->state_req = NGD_STATE_START;

            http->recved_each = 0;


            /*
             * Reset request metadata.
             */
            http->smethod = NULL;
            http->suri = NULL;
            http->sver = NULL;

            http->method_start = NULL;
            http->method_end = NULL;

            http->uri_start = NULL;
            http->uri_end = NULL;

            http->ver_start = NULL;
            http->ver_end = NULL;


            http->key_start = NULL;
            http->key_end = NULL;

            http->value_start = NULL;
            http->value_end = NULL;


            http->content_length = 0;

            http->on_zlib = false;
            http->on_chunk = false;

            http->on_body_file = false;

            http->body_recved = 0;

            http->chunk_size = 0;
            http->chunk_data_recved = 0;

            http->offset = 0;


            /*
             * Temporary file belongs to the old request.
             */
            if (http->fd_temp != -1) {

                close(http->fd_temp);
                http->fd_temp = -1;
            }


            /*
             * Reset body transfer buffer.
             */
            http->bodybuf->pos = http->bodybuf->start;
            http->bodybuf->last = http->bodybuf->start;


            /*
             * Reset output buffer.
             */
            http->outbuf->pos = http->outbuf->start;
            http->outbuf->last = http->outbuf->start;


            /*
             * Header list must be initialized for this request.
             *
             * Use your real list initialization API here.
             */
            ngd_list_init(&http->headers);


            state = ps_reqline;
            http->state = state;

            continue;



        /*
         * =====================================================
         * REQUEST LINE
         * =====================================================
         */
        case ps_reqline:

            ret = ngd_http_parse_reqline(http);


            if (ret == NGD_OK) {

                /*
                 * Parser itself resets state_req.
                 */

                state = ps_headers;
                http->state = state;

                continue;
            }


            if (ret == NGD_ERR) {

                /*
                 * Bad request cannot remain keep-alive.
                 */
                http->on_keep_alive = false;

                /*
                 * Build a normal 400 response.
                 */
                state = ps_build_resp;
                http->state = state;

                continue;
            }


            /*
             * Parser needs more bytes.
             */
            if (ret == NGD_AGAIN) {

                /*
                 * Only read when event layer says readable.
                 */
                if (!ngd_conn_on_read(c)) {

                    http->state = state;
                    return;
                }


                ret = ngd_http_read_request(c);


                if (ret == NGD_OK) {
                    continue;
                }


                if (ret == NGD_AGAIN) {

                    http->state = state;
                    return;
                }


                /*
                 * Input buffer completely full or fatal read error.
                 */
                http->on_keep_alive = false;

                state = ps_build_resp;
                http->state = state;

                continue;
            }


            state = ps_close;
            continue;



        /*
         * =====================================================
         * HEADERS
         * =====================================================
         */
        case ps_headers:

            ret = ngd_http_parse_headers(http);


            if (ret == NGD_OK) {

                /*
                 * Header parser itself resets state_req.
                 */


                if (http->on_chunk ||
                    http->content_length > 0)
                {
                    state = ps_body;

                } else {

                    state = ps_build_resp;
                }


                http->state = state;

                continue;
            }


            if (ret == NGD_ERR) {

                http->on_keep_alive = false;

                state = ps_build_resp;
                http->state = state;

                continue;
            }


            if (ret == NGD_AGAIN) {

                /*
                 * Request line + headers must fit in 16 KB.
                 */

                if (b->last == b->end &&
                    b->pos == b->start)
                {
                    http->on_keep_alive = false;

                    state = ps_build_resp;
                    http->state = state;

                    continue;
                }


                if (!ngd_conn_on_read(c)) {

                    http->state = state;
                    return;
                }


                ret = ngd_http_read_request(c);


                if (ret == NGD_OK) {
                    continue;
                }


                if (ret == NGD_AGAIN) {

                    http->state = state;
                    return;
                }


                http->on_keep_alive = false;

                state = ps_build_resp;
                http->state = state;

                continue;
            }


            state = ps_close;
            continue;



        /*
         * =====================================================
         * REQUEST BODY
         * =====================================================
         */
        case ps_body:

            /*
             * parse_body() must:
             *
             * - consume bytes from inbuf
             * - use bodybuf as transfer buffer
             * - store <= 16 KB bodies appropriately
             * - stream larger bodies to fd_temp
             * - preserve state_req on NGD_AGAIN
             *
             * It must NOT reject a body simply because the
             * body itself exceeds inbuf size.
             */
            ret = ngd_http_parse_body(http);


            if (ret == NGD_OK) {

                /*
                 * Body component resets state_req itself.
                 */

                state = ps_build_resp;
                http->state = state;

                continue;
            }


            if (ret == NGD_ERR) {

                http->on_keep_alive = false;

                state = ps_build_resp;
                http->state = state;

                continue;
            }


            if (ret == NGD_AGAIN) {

                /*
                 * There may already be body bytes in inbuf.
                 *
                 * parse_body() is responsible for consuming them
                 * before returning AGAIN.
                 */


                if (!ngd_conn_on_read(c)) {

                    http->state = state;
                    return;
                }


                ret = ngd_http_read_request(c);


                if (ret == NGD_OK) {
                    continue;
                }


                if (ret == NGD_AGAIN) {

                    http->state = state;
                    return;
                }


                state = ps_close;
                continue;
            }


            state = ps_close;
            continue;



        /*
         * =====================================================
         * BUILD RESPONSE
         * =====================================================
         */
        case ps_build_resp:

            /*
             * Only prepare:
             *
             * - status
             * - headers
             * - source file
             * - response metadata
             *
             * Do NOT send here.
             * Do NOT compress the entire response here.
             */
            ret = ngd_http_build_resp(http);


            if (ret != NGD_OK) {

                /*
                 * Cannot safely prepare response.
                 */
                state = ps_close;
                continue;
            }


            state = ps_send_resp;
            http->state = state;

            continue;



        /*
         * =====================================================
         * SEND RESPONSE
         * =====================================================
         */
        case ps_send_resp:

            /*
             * send_resp() owns streaming behavior:
             *
             * 1. Send existing outbuf first.
             *
             * 2. If outbuf still has unsent data:
             *      return NGD_AGAIN.
             *
             * 3. Only when outbuf is empty:
             *      read next source data.
             *
             * 4. If gzip enabled:
             *      gzip next source part into outbuf.
             *
             * 5. Preserve gzip state and source position
             *    across NGD_AGAIN.
             */
            if (!ngd_conn_on_write(c)) {

                http->state = state;
                return;
            }


            ret = ngd_http_send_resp(http);


            if (ret == NGD_OK) {

                /*
                 * Response completely finished.
                 */

                if (http->on_keep_alive) {

                    /*
                     * Preserve unread bytes in inbuf.
                     */
                    state = ps_start;
                    http->state = state;

                    continue;
                }


                state = ps_close;
                continue;
            }


            if (ret == NGD_AGAIN) {

                /*
                 * Preserve:
                 *
                 * - outbuf
                 * - gzip context
                 * - source position
                 * - state_req
                 */

                http->state = state;
                return;
            }


            state = ps_close;
            continue;



        /*
         * =====================================================
         * CLOSE
         * =====================================================
         */
        case ps_close:

            ngd_http_close_conn(c);
            return;

        }
    }
}
//new
void
ngd_http_handle_conn(ngd_conn_t *c)
{
    ngd_http_t *http;
    ngd_buf_t *b;

    int ret;
    size_t n;

    enum {
        ps_start = NGD_STATE_START,
        ps_reqline,
        ps_headers,
        ps_body,
        ps_build_resp,
        ps_send_resp,
        ps_close
    } state;


    http = ngd_conn_get_data(c);
    b = http->inbuf;

    state = http->state;


    for (;;) {

        switch (state) {

        case ps_start:

            /*
             * Do not touch unread input.
             *
             * [start, pos) consumed
             * [pos, last)  unread
             */

            http->state_req = NGD_STATE_START;

            http->recved_each = 0;

            http->smethod = NULL;
            http->suri = NULL;
            http->sver = NULL;

            http->method_start = NULL;
            http->method_end = NULL;

            http->uri_start = NULL;
            http->uri_end = NULL;

            http->ver_start = NULL;
            http->ver_end = NULL;

            http->key_start = NULL;
            http->key_end = NULL;

            http->value_start = NULL;
            http->value_end = NULL;

            http->content_length = 0;

            http->on_zlib = false;
            http->on_keep_alive = false;
            http->on_chunk = false;

            http->on_body_file = false;

            http->offset = 0;

            http->chunk_size = 0;
            http->chunk_data_recved = 0;

            /*
             * The old body temp file should already be finished
             * before starting the next request.
             */
            if (http->fd_temp != -1) {
                close(http->fd_temp);
                http->fd_temp = -1;
            }

            /*
             * Reset output buffer.
             */
            http->outbuf->pos = http->outbuf->start;
            http->outbuf->last = http->outbuf->start;


            ngd_list_init(&http->headers);


            state = ps_reqline;
            http->state = state;

            continue;


        case ps_reqline:

            ret = ngd_http_parse_reqline(http);

            if (ret == NGD_OK) {

                state = ps_headers;
                http->state = state;

                continue;
            }


            if (ret == NGD_ERR) {

                http->on_keep_alive = false;

                state = ps_build_resp;
                http->state = state;

                continue;
            }


            /*
             * Parser needs more input.
             */
            if (ret != NGD_AGAIN) {
                state = ps_close;
                continue;
            }


            /*
             * Request line must fit in the fixed 16 KB buffer.
             */
            if (b->last == b->end &&
                b->pos == b->start)
            {
                http->on_keep_alive = false;

                state = ps_build_resp;
                http->state = state;

                continue;
            }


            /*
             * Compact consumed bytes if necessary.
             */
            if (b->last == b->end &&
                b->pos != b->start)
            {
                n = b->last - b->pos;

                memmove(b->start, b->pos, n);

                b->pos = b->start;
                b->last = b->start + n;
            }


            ret = ngd_conn_recv(
                c,
                b->last,
                b->end - b->last,
                &n
            );


            if (ret == NGD_OK) {

                b->last += n;

                continue;
            }


            if (ret == NGD_AGAIN) {

                http->state = state;
                return;
            }


            if (ret == NGD_CLOSED) {

                state = ps_close;
                continue;
            }


            state = ps_close;
            continue;



        case ps_headers:

            ret = ngd_http_parse_headers(http);

            if (ret == NGD_OK) {

                if (http->on_chunk ||
                    http->content_length > 0)
                {
                    state = ps_body;

                } else {

                    state = ps_build_resp;
                }

                http->state = state;

                continue;
            }


            if (ret == NGD_ERR) {

                http->on_keep_alive = false;

                state = ps_build_resp;
                http->state = state;

                continue;
            }


            if (ret != NGD_AGAIN) {
                state = ps_close;
                continue;
            }


            /*
             * Compact input if needed.
             */
            if (b->last == b->end) {

                if (b->pos != b->start) {

                    n = b->last - b->pos;

                    memmove(b->start, b->pos, n);

                    b->pos = b->start;
                    b->last = b->start + n;
                }


                /*
                 * Still no space.
                 *
                 * Request line + headers exceeded 16 KB.
                 */
                if (b->last == b->end) {

                    http->on_keep_alive = false;

                    state = ps_build_resp;
                    http->state = state;

                    continue;
                }
            }


            ret = ngd_conn_recv(
                c,
                b->last,
                b->end - b->last,
                &n
            );


            if (ret == NGD_OK) {

                b->last += n;

                continue;
            }


            if (ret == NGD_AGAIN) {

                http->state = state;
                return;
            }


            state = ps_close;
            continue;



        case ps_body:

            ret = ngd_http_parse_body(http);

            if (ret == NGD_OK) {

                state = ps_build_resp;
                http->state = state;

                continue;
            }


            if (ret == NGD_ERR) {

                http->on_keep_alive = false;

                state = ps_build_resp;
                http->state = state;

                continue;
            }


            if (ret != NGD_AGAIN) {

                state = ps_close;
                continue;
            }


            /*
             * Body parser should consume body bytes from inbuf.
             *
             * Therefore it is allowed to compact the buffer
             * and continue streaming.
             */
            if (b->last == b->end) {

                if (b->pos != b->start) {

                    n = b->last - b->pos;

                    memmove(b->start, b->pos, n);

                    b->pos = b->start;
                    b->last = b->start + n;
                }


                /*
                 * Body parser returned AGAIN without consuming
                 * enough data to create space.
                 *
                 * That is a parser/body handling error.
                 */
                if (b->last == b->end) {

                    state = ps_close;
                    continue;
                }
            }


            ret = ngd_conn_recv(
                c,
                b->last,
                b->end - b->last,
                &n
            );


            if (ret == NGD_OK) {

                b->last += n;

                continue;
            }


            if (ret == NGD_AGAIN) {

                http->state = state;
                return;
            }


            state = ps_close;
            continue;



        case ps_build_resp:

            ret = ngd_http_build_resp(http);

            if (ret != NGD_OK) {
                state = ps_close;
                continue;
            }


            state = ps_send_resp;
            http->state = state;

            continue;



        case ps_send_resp:

            /*
             * Write is enabled by HTTP when needed.
             *
             * We don't ask "is writable?" here.
             *
             * send() itself tells us whether it can progress.
             */
            ret = ngd_http_send_resp(http);


            if (ret == NGD_OK) {

                /*
                 * Entire response finished.
                 */

                if (http->on_keep_alive) {

                    state = ps_start;
                    http->state = state;

                    continue;
                }


                state = ps_close;
                continue;
            }


            if (ret == NGD_AGAIN) {

                http->state = state;

                return;
            }


            state = ps_close;
            continue;



        case ps_close:
        default:

            ngd_http_close_conn(c);
            return;
        }
    }
}
