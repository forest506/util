#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>

#include <iostream>

int main()
{

    int server_sockfd, client_sockfd;
    int server_len;
    struct sockaddr_in server_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == server_sockfd)
    {
        std::cout << "create socket error" << std::endl;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(5002);

    server_len = sizeof(server_address);

    int rv = 0;

    rv = bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    if (-1 == rv)
    {
        std::cout << "bind error" << std::endl;
    }

    rv = listen(server_sockfd, 5);
    if (-1 == rv)
    {
        std::cout << "listen errror" << std::endl;
    }

    char ch;

    int sock_count = 0;

    while (1)
    {
        std::cout << "server waiting" << std::endl;

        client_sockfd = accept(server_sockfd, (struct sockaddr *)NULL, NULL);
        if(-1 == client_sockfd){
            std::cout << "server accept failed" << std::endl;
        }

        sock_count++;
        std::cout << "sock count:" << sock_count << std::endl;

        if (0 == fork())
        { //子进程
            sockaddr client_address;
            socklen_t len = 0;
            //int rv = getpeername(client_sockfd, (sockaddr *)&client_address, (socklen_t *)&len);
            //printf("client:%s", client_address.sa_data);

            ssize_t length = 0;

            while (ch < 'Z')
            {
                length =  read(client_sockfd, &ch, 1);
                if(0 == length)
                {
                    std::cout << "server eof" << std::endl;
                }else if(-1 == length){
                    std::cout << "server read error" << std::endl;
                }else if(length > 0){
                    std::cout << "server read bytes:" << length << std::endl;
                }

                std::cout << "server read: " <<  ch << std::endl;

                ch++;
                length = write(client_sockfd, &ch, 1);
                if(length > 0){
                    std::cout << "server write bytes:" << length << std::endl;
                }
            }

            close(client_sockfd);
        }
        else
        { //父进程
            close(client_sockfd);
        }
    }
}
