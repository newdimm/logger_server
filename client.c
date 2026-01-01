#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>

#include "protocol.h"

int start_client(const char *host, int port_no)
{
    struct addrinfo hints, *res, *result;
    char service[10];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME; // request to return the official name of the host in .ai_canonname

    snprintf(service, 10, "%u", port_no);

    if (getaddrinfo(host, service, &hints, &result) < 0)
    {
        perror("getaddrinfo()");
        return -1;
    }
    
    printf("getaddrinfo()\n");

    res = result;

    while (res)
    {
        printf("try connect to %s\n", res->ai_canonname);

        int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if (s >= 0)
        {
            printf("socket()\n");

            if (connect(s, res->ai_addr, res->ai_addrlen) >= 0)
            {
                printf("connect()\n");

                char data[256];

                int n;

                while ( (n = read(s, data, sizeof(data))) > 0)
                {
                    data[n] = 0;
                    printf("received: <%s>\n", data);
                }

                printf("done\n");

                close(s);
                break;
            }
            perror("connect()");
            close(s);
        }
        perror("socket()");

        res = res->ai_next;
    }

    freeaddrinfo(result);

    return 0;
}

int main(int argc, char *argv[])
{
    printf("client\n");

    start_client("localhost", PORT_NO);

    return 0;
}

