#include <sys/socket.h> 
#include <netinet/in.h>   //sockaddr_in
#include <arpa/inet.h>    //inet_pon
#include <unistd.h>       //fork,read,write
#include <stdlib.h>       //exit(0)
#include <errno.h>
#include <sys/types.h>
#include <sys/epoll.h>    //epoll
#include <sys/time.h>  
#include <string.h>       //bzero
#include <stdio.h>
#include <sys/stat.h>     //fstat

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

void str_cli(FILE* fp, int sockfd) {
    int epollfd, i, n, nev, maxevents, stdineof=0, isfile;
    char buf[MAXBUFSIZE];
    struct epoll_event ev, events[2];
    struct stat st;

    isfile = ((fstat(fileno(fp), &st) == 0) && (st.st_mode & S_IFMT) == S_IFREG);
    
    epollfd = epoll_create(2);

    ev.data.fd = fileno(fp);
    ev.events = EPOLLIN | EPOLLET;// EPOLLET(边缘触发，一次), EPOLLLT(水平触发，多次)
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fileno(fp), &ev); 

    ev.data.fd = sockfd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev); 
    
    maxevents = 2;
    for ( ; ; ) {
        //2、确定是否有所关注的事件发生
        nev = epoll_wait(epollfd, events, maxevents, INFTIM);
        for (i = 0; i < nev; ++i) {
            if (events[i].data.fd == sockfd) {
                if ( (n = read(sockfd, buf, MAXBUFSIZE)) == 0 ) {
                    if (stdineof == 1) 
                        return;
                    else
                        exit_msg("str_cli:server terminated prematurely");
                }
                write(fileno(stdout), buf, n); //stdout使用write即可，不必writen
            }
            if (events[i].data.fd == fileno(fp)) {
                n = read(fileno(fp), buf, MAXBUFSIZE);
                if (n > 0)
                    writen(sockfd, buf, n);
                if (n == 0 || (isfile && n == events[i].data.u32)) {
                    stdineof = 1;
                    shutdown(sockfd, SHUT_WR);
                    //3、DELETE事件
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, &ev);  //DELETE
                    --maxevents;
                }
            }
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