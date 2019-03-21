#include <sys/socket.h> 
#include <netinet/in.h>   //sockaddr_in
#include <unistd.h>       //fork,read,write
#include <stdlib.h>       //exit(0)
#include <errno.h>
#include <string.h>       //bzero
#include <poll.h>         //poll
#include <limits.h>       //OPEN_MAX
#include <stdio.h>

const int SERV_PORT = 9527;
const int LISTENQ = 5;
const int MAXBUFSIZE = 1024;

const int INFTIM = -1; //据说INFTIM被定义在<poll.h>中，但是没找到，<sys/stropts.h>中也没有

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
    int i, maxi, listenfd, connfd, sockfd;
    int nready;
    ssize_t n;
    char buf[MAXBUFSIZE];
    socklen_t clilen;
    struct pollfd client[OPEN_MAX]; //#define OPEN_MAX 10240 (Mac OS X)
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    if ( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 ) 
        exit_msg("bind error");

    listen(listenfd, LISTENQ);

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM; //(优先级)普通数据可读
    for (i =  1; i < OPEN_MAX; ++i)
        client[i].fd = -1;
    maxi = 0;
    for ( ; ; ) {
        nready = poll(client, maxi + 1, INFTIM);
        if (client[0].revents & POLLRDNORM) {
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);

            for (i = 1; i < OPEN_MAX; ++i) {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    break;
                }
            }
            if (i == OPEN_MAX) 
                exit_msg("too many clients");
            client[i].events = POLLRDNORM;
            if (i > maxi) maxi = i;
            if (--nready <= 0) continue;
        }
        for (i = 1; i <= maxi; ++i) {
            if ( (sockfd = client[i].fd) < 0 ) 
                continue;
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ( (n = read(sockfd, buf, MAXBUFSIZE)) < 0 ) {
                    if (errno == ECONNRESET) {
                        close(sockfd);
                        client[i].fd = -1;
                    }
                    else {
                        exit_msg("read error");
                    }
                } 
                else if (n == 0) {
                    close(sockfd);
                    client[i].fd = -1;
                }
                else {
                    writen(sockfd, buf, n);//DIY: writen = write + while
                }
                if (--nready <= 0)
                    break;
            }
        }
    }

    return 0;
}