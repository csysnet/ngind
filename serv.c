#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define BACKLOG 10
#define BUFF_SIZE 4096

int start();

int main()
{
    start();
}


int start()
{
    int serv_fd, cli_fd;
    int n_read;
    char buff[BUFF_SIZE];
    struct sockaddr_in addr;
    const char *MSG = "HELLO, cli";

    serv_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = PORT;

    bind(serv_fd, (struct sockaddr *)&addr, sizeof(addr));

    listen(serv_fd, BACKLOG);

    for (;;)
    {
        cli_fd = accept(serv_fd, (struct sockaddr *)NULL, NULL);
        for (;;)
        {
            n_read = read(cli_fd, buff, BUFF_SIZE);
            buff[n_read] = '\0';
            printf("cli: %s\n", buff);

            write(cli_fd, MSG, strlen(MSG));
            printf("serv: %s\n", MSG);
            sleep(1);
        }
        close(cli_fd);
    }

}
