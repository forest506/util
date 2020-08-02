#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    char ch = 'A';

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd)
    {
        std::cout << "client socket error" << std::endl;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(5002);

    len = sizeof(address);

    result = connect(sockfd, (struct sockaddr *)&address, len);
    if (result == -1)
    {
        std::cout << "oops: client1" << std::endl;
        exit(1);
    }

    int count = 10;
    ssize_t length = 0;
    while (count-- > 0)
    {
        length = write(sockfd, &ch, 1);
        if (length > 0)
        {
            std::cout << "server write bytes:" << length << std::endl;
        }

        length = read(sockfd, &ch, 1);
        if (0 == length)
        {
            std::cout << "client eof" << std::endl;
        }
        else if (-1 == length)
        {
            std::cout << "client read error" << std::endl;
        }
        else if (length > 0)
        {
           std::cout << "client read bytes:" << length << std::endl;
        }

        std::cout << "char from server = " <<  ch << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    close(sockfd);

    exit(0);
}