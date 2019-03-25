#include <sys/socket.h> 
#include <netinet/in.h>   //sockaddr_in
#include <unistd.h>       //fork,read,write
#include <stdlib.h>       //exit(0)
#include <errno.h>
#include <string.h>       //bzero
#include <sys/epoll.h>    //epoll
#include <limits.h>       //OPEN_MAX
#include <stdio.h>

const int SERV_PORT = 9527;
const int LISTENQ = 5;
const int MAXBUFSIZE = 1024;
const int OPEN_MAX = 10240;
const int INFTIM = -1; //TIMEOUT

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
    int epollfd, i, nev, n, maxevents, listenfd, connfd, sockfd;
    char buf[MAXBUFSIZE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    struct epoll_event ev, events[OPEN_MAX];//#define OPEN_MAX 10240 (Mac OS X)

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if ( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 ) 
        exit_msg("bind error");

    listen(listenfd, LISTENQ);

    epollfd = epoll_create(OPEN_MAX);
    
    ev.data.fd = listenfd;
    ev.events = EPOLLIN | EPOLLET;// EPOLLET(边缘触发，一次), EPOLLLT(水平触发，多次)
    //1、注册所关注的事件
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev); 
    maxevents = 1;
    for ( ; ; ) {
        //2、确定是否有所关注的事件发生
        nev = epoll_wait(epollfd, events, maxevents, INFTIM); 
        for (i = 0; i < nev; ++i) {
            if (events[i].data.fd == listenfd && maxevents < OPEN_MAX) {
                clilen = sizeof(cliaddr);
                connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
                ev.data.fd = connfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev); 
                ++maxevents;
            } 
            else {
                sockfd = events[i].data.fd;
                if ( (n = read(sockfd, buf, MAXBUFSIZE)) < 0 ) {
                    if (errno == ECONNRESET) {
                        close(sockfd);
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, &ev);  //DELETE
                        --maxevents;
                    }
                    else {
                        exit_msg("read error");
                    }
                } 
                else if (n == 0) {
                    close(sockfd);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, &ev);  //DELETE
                    --maxevents;
                }
                else {
                    writen(sockfd, buf, n);//DIY: writen = write + while
                }
            }
        }
    }

    return 0;
}