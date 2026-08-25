#ifndef NGD_STR_T
#define NGD_STR_T
//
#include "ngd_core.h"
//note!!!
#define NGD_STR(s) \
    (ngd_str_t){ .data = (s), .len = sizeof(s) - 1 }
//

//
#endif
