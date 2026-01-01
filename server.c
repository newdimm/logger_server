#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "protocol.h"

static int start_server(int port_no)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0)
    {
        perror("socket()");
        return -1;
    }

    printf("socket()\n");

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    /* The sin_port and sin_addr members are stored in network byte order */
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port_no);

    if (bind(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind()");
        return -1;
    }

    printf("bind()\n");

    if (listen(s, 100) < 0)
    {
        perror("listen()");
        return -1;
    }

    printf("listen()\n");

    while (1)
    {
        struct sockaddr_in clientaddr;

        printf("waiting for connection...\n");

        socklen_t addrlen = sizeof(clientaddr);

        int fd = accept(s, (struct sockaddr *)&clientaddr, &addrlen);

        if (fd < 0)
        {
            perror("accept()");
            return -1;
        }

        char buff[255];

        printf("accept() connection from %s port %d\n",
                inet_ntop(AF_INET, &clientaddr.sin_addr, buff, sizeof(buff)),
                ntohs(clientaddr.sin_port));
        
        time_t ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

        int n = write(fd, buff, strlen(buff));
        printf("write returned %d\n", n);
        sleep(1);

        printf("close()\n");
        close(fd);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    printf("Hello, World!\n");

    start_server(PORT_NO);

    return 0;
}

