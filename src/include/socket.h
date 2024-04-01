#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <unistd.h>

int SocketIPV4(){
    again:
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){

        if(errno == EINTR){
            goto again;
        }
        perror("Socket creation Failed");
        exit(1);
    }

    return sockfd;

}

int ListeningSocket(){

    int sockfd = SocketIPV4();
   
    struct sockaddr_in servAddr = {};
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(8080);
    servAddr.sin_addr.s_addr =  INADDR_ANY;

  
    if(bind(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
        perror("Cannot Bind Socket");
        exit(1);
    }

    if(listen(sockfd, 1024) < 0){
        perror("Cannot Listen on Socket");
        exit(1);
    }    

    return sockfd;

}

int Accept(int sockfd){
    again:
    int connfd = accept(sockfd, nullptr, nullptr);
    if(connfd < 0){
        if(errno == EINTR){
            goto again;
        }
        perror("Accept had an error");
        exit(1);
    }

    return connfd; 
}


void Writen(int fd, const char* buf, int n){

    int nLeft = n;
    const void *ptr = buf;
    while(nLeft > 0){
        again:
        int nWritten = write(fd, ptr, nLeft);
        if(nWritten < 0){
            if(errno == EINTR) goto again;
            perror("Write error");
            exit(1);
        }

        if(nWritten == 0){

            fputs("Socket is closed\n", stdout);
            exit(1);
        }

        nLeft -= nWritten;
        ptr += nWritten;
    }


    
}

void Readn(int fd, char *buf, int n){

    int nLeft = n;
    void *ptr = buf;

    while(nLeft > 0){
        again:
        int nRead = read(fd, ptr, nLeft);
        if (nRead < 0){
            if(errno == EINTR){
                goto again;
            }
            perror("Read Error");
            exit(1);
        }

        if(nRead == 0){
            fputs("Socket is closed\n", stdout);
            exit(1);
        }

        nLeft -= nRead;
        ptr += nRead;
    }
    
}


void ReadLine(int fd, char *buf, int n){
    int nLeft = n - 2;
    char *ptr = buf;

    while(nLeft > 0){
        again:
        int nRead = read(fd, ptr, 1);
        if (nRead < 0){
            if(errno == EINTR){
                goto again;
            }
            perror("Read Error");
            exit(1);
        }

        if(nRead == 0){
            fputs("Socket is closed", stdout);
            exit(1);
        }

        
        if (*ptr == '\n' || *ptr == '\0'){
            break;
        }
        nLeft -= nRead;
        ptr += nRead;
    }

    *ptr++ = '\n';
    *ptr = '\0';

}
