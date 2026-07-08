#include "ssl.h"


int ssl_ctx_init(ssl_ctx_t *ctx) {return NGD_OK;}
int ssl_ctx_cert(ssl_ctx_t *ctx, str_t *cert, str_t *key) {return NGD_OK;}
//
int ssl_conn_init(ssl_conn_t *sc, ssl_ctx_t *ctx) {return NGD_OK;}
int ssl_conn_setfd(ssl_conn_t *sc, int fd) {return NGD_OK;}
