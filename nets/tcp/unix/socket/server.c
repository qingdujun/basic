#include <sys/socket.h> 
#include <netinet/in.h>   //sockaddr_in
#include <unistd.h>       //fork,read,write
#include <stdlib.h>       //exit(0)
#include <errno.h>
#include <string.h>       //bzero
#include <stdio.h>

const int SERV_PORT = 9527;
const int LISTENQ = 5;
const int MAXBUFFSIZE = 1024;
const int MAXLINE = 128;

ssize_t readline(int fd, void* vptr, size_t max_length) {
    ssize_t n, rc;
    char c, *ptr;
    ptr = vptr;

    for (n = 1; n < max_length; ++n) {
        if ( (rc = read(fd, &c, 1)) == 1 ) {
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

void sig_chld(int signo) {
    pid_t pid;
    int stat;

    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0 )
        printf("child pid=%d terminated\n", pid);//警告: printf为不可重入函数
    return;  //显示`return`接受中断位置处的指令
}

void str_echo(int sockfd) {
    ssize_t n;
    char buf[MAXLINE];

    for ( ; ; ) {
        while ( (n = readline(sockfd, buf, MAXLINE)) > 0 ) 
            writen(sockfd, buf, n);
        if (n < 0 && errno == EINTR) 
            continue;
        break;
    }
}

int main(int argc, char* argv[]) {
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if ( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 ) 
        exit_msg("bind error");

    listen(listenfd, LISTENQ);

    signal(SIGCHLD, &sig_chld);

    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        if ( (connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) < 0 ) {
            if (errno == EINTR) 
                continue;
            exit_msg("accept error");
        }
        if ( (childpid = fork()) == 0 ) {//child fork
            close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        close(connfd);
    }

    return 0;
}