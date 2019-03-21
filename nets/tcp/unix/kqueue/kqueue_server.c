#include <sys/socket.h> 
#include <netinet/in.h>   //sockaddr_in
#include <unistd.h>       //fork,read,write
#include <stdlib.h>       //exit(0)
#include <errno.h>
#include <string.h>       //bzero
#include <sys/event.h>    //kevent
#include <limits.h>       //OPEN_MAX
#include <stdio.h>
#include <sys/stat.h>     //fstat

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
    int kq, i, nev, n, nevents, listenfd, connfd, sockfd;
    char buf[MAXBUFSIZE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    struct kevent ev, kev[OPEN_MAX];//#define OPEN_MAX 10240 (Mac OS X)
    struct timespec ts;
    struct stat st;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if ( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 ) 
        exit_msg("bind error");

    listen(listenfd, LISTENQ);

    EV_SET(&ev, listenfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kq = kqueue();
    ts.tv_sec = ts.tv_nsec = 0;
    //1、注册所关注的事件
    kevent(kq, &ev, 1, NULL, 0, &ts);  //&ts=0，非阻塞
    nevents = 1;
    for ( ; ; ) {
        //2、确定是否有所关注的时间发生
        nev = kevent(kq, NULL, 0, kev, nevents, NULL); //timeout=NULL表示阻塞
        for (i = 0; i < nev; ++i) {
            if (kev[i].ident == listenfd && nevents < OPEN_MAX) {
                clilen = sizeof(cliaddr);
                connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
                EV_SET(&ev, connfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                kevent(kq, &ev, 1, NULL, 0, &ts);
                ++nevents;
            } 
            else {
                sockfd = kev[i].ident;
                if ( (n = read(sockfd, buf, MAXBUFSIZE)) < 0 ) {
                    if (errno == ECONNRESET) {
                        close(sockfd);
                        EV_SET(&ev, sockfd, 0, EV_DELETE, 0, 0, NULL); //DELETE    
                        kevent(kq, &ev, 1, NULL, 0, &ts);
                        --nevents;
                    }
                    else {
                        exit_msg("read error");
                    }
                } 
                else if (n == 0) {
                    close(sockfd);
                    EV_SET(&ev, sockfd, 0, EV_DELETE, 0, 0, NULL); //DELETE    
                    kevent(kq, &ev, 1, NULL, 0, &ts);
                    --nevents;
                }
                else {
                    writen(sockfd, buf, n);//DIY: writen = write + while
                }
            }
        }
    }

    return 0;
}