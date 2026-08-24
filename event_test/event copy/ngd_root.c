#include "ngd_conn.h"
#include "ngd_http.h"

int
main()
{
    ngd_conn_module_init(ngd_http_init_conn);
    ngd_conn_module_start();
}
