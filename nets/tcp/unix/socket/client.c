#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h>  //inet_pon
#include <unistd.h>     //fork,read,write
#include <string.h>     //strlen
#include <stdlib.h>     //exit(0)
#include <errno.h>
#include <stdio.h>      //FILE*

const int SERV_PORT = 9527;
const int LISTENQ = 5;
const int MAXBUFFSIZE = 1024;
const int MAXLINE = 128;

ssize_t reandn(int fd, void* vptr, size_t n) {
    ssize_t nread;
    char *ptr;
    size_t nleft = n;
    ptr = vptr;

    while (nleft > 0) {
        if ( (nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0;
            else 
                return -1;
        }
        else if (nread == 0) {
            break;  //EOF
        }
        nleft -= nread;
        ptr += nread;
    }
    return (n - nleft);
}

ssize_t readline(int fd, void* vptr, size_t max_length) {
    ssize_t n, rc;
    char c, *ptr;
    ptr = vptr;

    for (n = 1; n < max_length; ++n) {
        if ( (rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n') 
                break;
        }
        else if (rc == 0) {
            *ptr = 0;
            return (n-1);
        }
        else {
            if (errno == EINTR) {
                --n;
                continue;
            }
            return -1;
        }
    }
    *ptr = 0;
    return n;
}

ssize_t writen(int fd, void const* vptr, size_t n) {
    ssize_t nwritten;
    char const* ptr;
    size_t nleft = n;
    ptr = vptr;

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

void str_cli(FILE* fp, int sockfd) {
    char send_line[MAXLINE], recv_line[MAXLINE];
    while ( fgets(send_line, MAXLINE, fp) != NULL ) {
        int n = writen(sockfd, send_line, strlen(send_line));
        if ( readline(sockfd, recv_line, MAXLINE) == 0 ) {
            perror("str_cli readline==0");
            exit(0);
        }
        fputs(recv_line, stdout);
    }
}

int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2) {
        perror("argv != 2");
        exit(0);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("conn err");
        exit(0);
    }

    str_cli(stdin, sockfd);

    return 0;
}