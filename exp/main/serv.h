#ifndef SERV_H
#define SERV_H

#define PORT 8080
#define BACKLOG 10
#define BUFF_SIZE 4096


const char *RES_OK = "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %d\r\n"
                "\r\n";

void
ps(void *buf, size_t buflen);

off_t
get_flen(int fd);

void
send_msg(int fd, void *buf, size_t buflen);

void
send_ok(int cli_fd, void *buf, size_t buflen, const char *fname);

ssize_t
recv_msg(int fd, void *buf, size_t buflen);

void
handle_cli(int cli_fd);

void
start();




#endif
