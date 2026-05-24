#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "./serv.h"


off_t
get_flen(int fd)
{
    struct stat st;

    fstat(fd, &st);

    return st.st_size;
}

void
send_msg(int fd, void *buf, size_t buflen)
{
    int nwrite, total;

    total = 0;
    while (total < buflen)
    {
        nwrite = write(fd, buf + total, buflen - total);
        ps(buf + total, buflen - total);
        total += nwrite;
    }
}

void
ps(void *buf, size_t buflen)
{
    for (int i = 0; i<buflen; i++)
        printf("%c", ((char*)buf)[i]);
}

void
send_ok(int cli_fd, void *buf, size_t buflen, const char *fname)
{
    int fd;
    ssize_t nread, nwrite;
    size_t twrite;
    off_t flen;

    fd = open(fname, O_RDONLY);
    flen = get_flen(fd);

    snprintf(buf, buflen, RES_OK, flen);

    send_msg(cli_fd, buf, strlen(buf));
    while (1)
    {
        nread = read(fd, buf, BUFF_SIZE);
        if (nread==0) break;
        send_msg(cli_fd, buf, nread);
    }
}

void
handle_cli(int cli_fd)
{
    char buf[BUFF_SIZE];
    send_ok(cli_fd, buf, sizeof(buf), "./index.html");
}

ssize_t
recv_msg(int fd, void *buf, size_t buflen)
{
    ssize_t n_read;
    size_t total;

    total = 0;
    while (total < buflen)
    {
        n_read = read(fd, (char *)buf + total, buflen - total);
        if (n_read == -1 || n_read == 0)
            return n_read;

        total += n_read;
    }
    return total;
}



int main()
{
    start();
}


void start()
{
    int serv_fd, cli_fd;
    int n_read;
    struct sockaddr_in addr;

    serv_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(serv_fd, (struct sockaddr *)&addr, sizeof(addr));

    listen(serv_fd, BACKLOG);

    while (1) {
        cli_fd = accept(serv_fd, (struct sockaddr *)NULL, NULL);

        handle_cli(cli_fd);
        close(cli_fd);
    }

}
