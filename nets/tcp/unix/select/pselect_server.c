#include <sys/socket.h> 
#include <netinet/in.h>   //sockaddr_in
#include <unistd.h>       //fork,read,write
#include <stdlib.h>       //exit(0)
#include <errno.h>
#include <string.h>       //bzero
#include <stdio.h>
#include <signal.h>
#include <sys/select.h>

const int SERV_PORT = 9527;
const int LISTENQ = 5;
const int MAXBUFSIZE = 1024;

void exit_msg(char* msg) {
    perror(msg);
    exit(0);
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

int main(int argc, char* argv[]) {
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE]; //FD_SETSIZE被定义在_fd_setsize.h中，默认值为1024 (MaC OS X)
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXBUFSIZE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    sigset_t newmask, oldmask, zeromask;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if ( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 ) 
        exit_msg("bind error");

    listen(listenfd, LISTENQ);

    maxfd = listenfd;
    maxi = -1;
    for (i =  0; i < FD_SETSIZE; ++i)
        client[i] = -1;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    for ( ; ; ) {
        rset = allset;

        sigemptyset(&zeromask);
        sigemptyset(&newmask);
        sigaddset(&newmask, SIGINT); //屏蔽SIGINT信号(即，Ctrl+C)

        sigprocmask(SIG_BLOCK, &newmask, &oldmask);

        if ( (nready = pselect(maxfd+1, &rset, NULL, NULL, NULL, &zeromask) < 0 ) { 
            if (errno == EINTR) 
                ;
        }

        if (FD_ISSET(listenfd, &rset)) { //a new client connection
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);

            for (i = 0; i < FD_SETSIZE; ++i) {
                if (client[i] < 0) {
                    client[i] = connfd;
                    break;
                }
            }
            if (i == FD_SETSIZE)
                exit_msg("too many clients");
            FD_SET(connfd, &allset);
            if (connfd > maxfd) maxfd = connfd;
            if (i > maxi) maxi = i;
            if (--nready <= 0) continue;
        }
        for (i = 0; i <= maxi; ++i) {
            if ( (sockfd=client[i]) < 0 )
                continue;
            if (FD_ISSET(sockfd, &rset)) {
                if ( (n = read(sockfd, buf, MAXBUFSIZE)) == 0 ) {
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }
                else {
                    writen(sockfd, buf, n);
                }
                if (--nready <= 0)
                    break;
            }
        }
    }

    return 0;
}