#include <iostream>
#include <socket.h>
#include <string.h>

using namespace std;
int main(){

    char buf[1024] = "Hello\0";
    int sockfd = ListeningSocket();
    
    int connfd;
    while(connfd = Accept(sockfd)){
        if(fork() == 0){
            close(sockfd);
            while(true){
                bzero(buf, 1024);
                ReadLine(connfd, buf, sizeof(buf));
                fputs(buf, stdout);
                Writen(connfd,buf,strlen(buf));

            }
        }
        close(connfd);

    }
    
    
    return 0;
}