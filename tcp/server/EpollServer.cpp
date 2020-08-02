#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <strings.h>
#include <arpa/inet.h>
#include <iostream>

#define MAXLINE 5
#define OPEN_MAX 100
#define LSITENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

void setnonblocking(int sock)
{

    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0)
    {
        perror("fcntl error");
        exit(1);
    }

    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts))
    {
        perror("fcntl error 2");
        exit(1);
    }
}

int main()
{

    int i, maxi, listenfd, connfd, sockfd, epfd, nfds, portnumber = SERV_PORT, rv = 0;
    ssize_t n;
    char line[MAXLINE];
    socklen_t clilen;

    struct epoll_event ev, events[20]; //ev用于注册事件，数组用于回传要出来的事情

    epfd = epoll_create(256);
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    setnonblocking(listenfd); //非阻塞

    //设置要出来的事件相关的文件描述符
    ev.data.fd = listenfd;
    //设置要处理的事件类型
    ev.events = EPOLLIN | EPOLLET;

    //注册epoll事件
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;

    char *local_addr = "127.0.0.1";
    inet_aton(local_addr, &(serveraddr.sin_addr));
    serveraddr.sin_port = htons(portnumber);
    rv = bind(listenfd, (sockaddr *)&serveraddr, sizeof(serveraddr));

    if (-1 == rv)
    {
        perror("bind error");
    }

    rv = listen(listenfd, LSITENQ);
    maxi = 0;

    for (;;)
    {

        //等待epoll事件的发生
        nfds = epoll_wait(epfd, events, 20, 500);
        //处理所发生的所有事件
        for (i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == listenfd) //如果新检测到一个socket连接，建立新的连接
            {
                connfd = accept(listenfd, (sockaddr *)&clientaddr, &clilen);
                if (connfd < 0)
                {
                    perror("connfd < 0");
                    exit(1);
                }
                setnonblocking(connfd);

                char *str = inet_ntoa(clientaddr.sin_addr);
                std::cout << "accept from" << str << std::endl;

                //设置用于读操作的文件描述符
                ev.data.fd = connfd;
                //设置用于注册的读操作事件
                ev.events = EPOLLIN | EPOLLET;

                //注册ev
                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            }
            else if (events[i].events & EPOLLIN)
            { //已经连接的用户，读取数据

                if ((sockfd = events[i].data.fd) < 0)
                {
                    continue;
                }

                if ((n = read(sockfd, line, MAXLINE)) < 0)
                {
                    if (errno == ECONNRESET)
                    {
                        close(sockfd);
                        events[i].data.fd = -1;
                    }
                    else if (n == 0)
                    {
                        close(sockfd);
                        events[i].data.fd = -1;
                    }
                    else
                    {
                        std::cout << "readline error" << std::endl;
                    }                    

                    line[n] = '/0';
                    std::cout << "read:" << line << std::endl;

                    //设置用于写操作的文件描述符
                    ev.data.fd = sockfd;
                    //设置用于注册的写操作事件
                    ev.events = EPOLLOUT | EPOLLET;
                }
            }
            else if (events[i].events & EPOLLOUT) //如果有数据发送
            {
                sockfd = events[i].data.fd;
                write(sockfd, line, n);
                //设置用于读操作的文件描述符
                ev.data.fd = sockfd;
                //设置用于注册的读操作事件
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
        }
    }
}