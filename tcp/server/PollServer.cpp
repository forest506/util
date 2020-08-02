#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <limits.h>
#include <poll.h>
#include <iostream>

#define OPEN_MAX 20
#define MAX_LINE 256

int main()
{

    int i, maxi, listenfd, connfd, sockfd;
    int nready;
    ssize_t n;
    char buf[MAX_LINE] = {0};

    socklen_t clilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cliaddr, servaddr;

    int rv = 0;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listenfd)
    {
        std::cout << "server create socket error" << std::endl;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    servaddr.sin_port = htons(5002);

    rv = bind(listenfd, (sockaddr *)&servaddr, sizeof(servaddr));
    if (-1 == rv)
    {
        std::cout << "server bind error" << std::endl;
    }

    rv = listen(listenfd, 0);
    if (-1 == rv)
    {
        std::cout << "server listen error" << std::endl;
    }

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;

    for (i = 1; i < OPEN_MAX; i++)
    {
        client[i].fd = -1;
    }

    maxi = 0;

    for (;;)
    {
        nready = poll(client, maxi + 1, -1);

        if (client[0].revents & POLLRDNORM)
        {
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (sockaddr *)&cliaddr, &clilen);

            for (i = 0; i < OPEN_MAX; i++)
            {
                if (client[i].fd < 0)
                {
                    client[i].fd = connfd;
                    break;
                }

                if (i == OPEN_MAX)
                {
                    std::cout << "too many clients" << std::endl;
                }

                client[i].events = POLLRDNORM;
                if (i > maxi)
                {
                    maxi = i;
                }

                if (--nready <= 0)
                {
                    continue;
                }

                for (i = 1; i <= maxi; i++)
                {
                    if ((sockfd = client[i].fd) < 0)
                    {
                        continue;
                    }

                    if (client[i].revents & (POLLRDNORM | POLLERR))
                    {
                        if ((n = read(sockfd, buf, MAX_LINE)) < 0)
                        {
                            if (errno == ECONNRESET)
                            {
                                close(sockfd);
                                client[i].fd = -1;
                            }
                            else
                            {
                                std::cout << "read error" << std::endl;
                            }
                        }
                        else if (0 == n)
                        {
                            close(sockfd);
                            client[i].fd = -1;
                        }
                        else
                        {
                            write(sockfd, buf, n);
                        }

                        if (--nready <= 0)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
}