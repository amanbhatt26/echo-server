#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

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

int ClientSocket(const char *ipaddress){

    int sockfd = SocketIPV4();

    struct sockaddr_in servAddr = {};
    servAddr.sin_family = AF_INET;
    struct in_addr s_addr;
    int aconvert;
    if((aconvert = inet_pton(AF_INET, ipaddress, &s_addr) ) <= 0){
        if(aconvert == 0){
            fputs("Invalid IP address\n", stdout);
            
        }
        else{
            perror("Cannot Convert IP address");
            
        }
        exit(1);
    }

    servAddr.sin_addr = s_addr;
    servAddr.sin_port = htons(8080);
    
    if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
        perror("Cannot Connect socket");
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

            perror("Socket is closed");
            // fputs("Socket is closed\n", stdout);
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
            perror("Socket is closed");
            // fputs("Socket is closed\n", stdout);
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
            perror("Socket is closed");
            // fputs("Socket is closed\n", stdout);
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

void sig_chld(int signo){

    pid_t pid;
    int stat;

    while(waitpid(-1,&stat,WNOHANG) > 0);

    return;

}

void RegisterSigChldHandler(){
    struct sigaction sga = {};
    sga.sa_handler = &sig_chld;
    sga.sa_flags = 0;
    // sigemptyset(&sga.sa_mask);

    sigaction(SIGCHLD, &sga, NULL);

}

void RegisterSigPipeHandler(){
    struct sigaction sga = {};
    sga.sa_handler = SIG_IGN;
    sga.sa_flags = 0;

    sigaction(SIGPIPE, &sga, NULL);
}