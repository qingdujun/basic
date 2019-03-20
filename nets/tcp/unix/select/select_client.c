#include <sys/socket.h> 
#include <netinet/in.h>   //sockaddr_in
#include <arpa/inet.h>  //inet_pon
#include <unistd.h>       //fork,read,write
#include <stdlib.h>       //exit(0)
#include <errno.h>
#include <string.h>       //bzero
#include <stdio.h>

const int SERV_PORT = 9527;
const int LISTENQ = 5;
const int MAXBUFSIZE = 1024;

int max(int a, int b) {
    return a > b ? a : b;
}

ssize_t writen(int fd, void const* vptr, size_t n) {
    ssize_t nwritten;
    size_t nleft = n;
    char const* ptr = vptr;

    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) <= 0 ) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

void exit_msg(char* msg) {
    perror(msg);
    exit(0);
}

void str_cli(FILE* fp, int sockfd) {
    fd_set rset;
    char buf[MAXBUFSIZE];
    int maxfdp1, n, stdineof = 0;

    FD_ZERO(&rset);
    for ( ; ; ) {
        if (stdineof == 0)
            FD_SET(fileno(fp), &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = max(fileno(fp), sockfd) + 1;
        select(maxfdp1, &rset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rset)) {
            if ( (n = read(sockfd, buf, MAXBUFSIZE)) == 0 ) {
                if (stdineof == 1) 
                    return;
                else
                    exit_msg("str_cli:server terminated prematurely");
            }
            write(fileno(stdout), buf, n); //stdout使用write即可，不必writen
        }
        if (FD_ISSET(fileno(fp), &rset)) {
            if ( (n = read(fileno(fp), buf, MAXBUFSIZE)) == 0 ) {
                stdineof = 1;
                shutdown(sockfd, SHUT_WR); //FIN
                FD_CLR(fileno(fp), &rset);
                continue;
            } 
            writen(sockfd, buf, n);
        }
    }
}

int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2)
        exit_msg("argv != 2");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
        exit_msg("conn err");//警告: connect请勿捕获EINTR

    str_cli(stdin, sockfd);

    return 0;
}