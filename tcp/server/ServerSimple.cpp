#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>

int main(){

    int server_sockfd, client_sockfd;
    int server_len;
    struct sockaddr_in server_address;
    
    
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);   
    if(-1 == server_sockfd){
        printf("create socket error");
    }    

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(5002);


    server_len = sizeof(server_address);

    int rv = 0;

    rv = bind(server_sockfd, (struct sockaddr*)&server_address, server_len);
    if(-1 == rv){
        printf("bind error");
    }

    rv = listen(server_sockfd, 5);
    if(-1 == rv){
        printf("listen errror");
    }

    char ch;

    while (1)
    {
        printf("server waiting\n");
        
        client_sockfd = accept(server_sockfd, (struct sockaddr*)NULL, NULL);

        read(client_sockfd, &ch, 1);
        ch++;


        write(client_sockfd, &ch, 1);
        close(client_sockfd);

    }
    


}
