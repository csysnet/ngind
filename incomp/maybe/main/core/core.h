#ifndef CORE_H
#define CORE_H
#include <stddef.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define RED     "\033[31m"
#define RESET   "\033[0m"

#define NGD_OK 0
#define NGD_ERR -1
#define NGD_AGAIN -2
#define NGD_STATE_START 0
typedef unsigned char u_char;


#endif
