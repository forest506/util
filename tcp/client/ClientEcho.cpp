
#include <cstdio>
#include <cstring>
#include <sys/socket.h>


#define MAX_LINE 10

void str_cli(FILE *fp, int sockfd){
    char sendline[MAX_LINE], recvline[MAX_LINE];

    while(NULL != fgets(sendline, MAX_LINE, fp)){

            send(sockfd, sendline, strlen(sendline));

    }

}

