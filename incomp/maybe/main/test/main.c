#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
//
int
main()
{
    char *buf;
    int fd;
    ssize_t n;
    //
    buf = malloc(1024);

    buf[100000] = '\0';
    //
    fprintf(stderr, "bef free: %p\n", buf);
    free(buf);
    fprintf(stderr, "af free: %p\n", buf);
    buf[102400] = 'd';
    fprintf(stderr, "succeed\n");
    //
}
