#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFF_SIZE 4096
#define ADDR "127.0.0.1"
#define PORT 8081

void start();

int main()
{
    printf("connecting to %d\n", PORT);
    start();

}

void start()
{
    int serv_fd;
    int n_read;
    struct sockaddr_in addr;
    char buff[BUFF_SIZE];
    const char MSG[] =
        "GET /DAT HTTP/1.1\r\n"
        "host: debian\r\n"
        "\r\n";

    serv_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = PORT;
    inet_pton(AF_INET, ADDR, &addr.sin_addr);

    connect(serv_fd, (struct sockaddr *)&addr, sizeof(addr));
    printf("connected\n");
    for (;;)
    {
        write(serv_fd, MSG, sizeof(MSG));
        printf("cli: %s\n", MSG);
        fflush(stdout);
        printf("out\n");

        n_read = read(serv_fd, buff, BUFF_SIZE);
        buff[n_read] = '\0';
        printf("serv: %s\n", buff);
    }
}
