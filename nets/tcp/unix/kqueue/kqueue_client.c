#include <sys/socket.h> 
#include <netinet/in.h>   //sockaddr_in
#include <arpa/inet.h>    //inet_pon
#include <unistd.h>       //fork,read,write
#include <stdlib.h>       //exit(0)
#include <errno.h>
#include <sys/types.h>
#include <sys/event.h>    //kqueue
#include <sys/time.h>  
#include <string.h>       //bzero
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

void str_cli(FILE* fp, int sockfd) {
    int kq, i, n, nev, stdineof=0, isfile;
    char buf[MAXBUFSIZE];
    struct kevent kev[2];
    struct timespec ts;
    struct stat st;

    isfile = ((fstat(fileno(fp), &st) == 0) && (st.st_mode & S_IFMT) == S_IFREG);
    
    EV_SET(&kev[0], fileno(fp), EVFILT_READ, EV_ADD, 0, 0, NULL);
    EV_SET(&kev[1], sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);

    kq = kqueue();
    ts.tv_sec = ts.tv_nsec = 0;
    //1、注册所关注的事件
    kevent(kq, kev, 2, NULL, 0, &ts);  //&ts=0，非阻塞

    for ( ; ; ) {
        //2、确定是否有所关注的时间发生
        nev = kevent(kq, NULL, 0, kev/*out*/, 2, NULL); //timeout=NULL表示阻塞
        for (i = 0; i < nev; ++i) {
            if (kev[i].ident == sockfd) {
                if ( (n = read(sockfd, buf, MAXBUFSIZE)) == 0 ) {
                    if (stdineof == 1) 
                        return;
                    else
                        exit_msg("str_cli:server terminated prematurely");
                }
                write(fileno(stdout), buf, n); //stdout使用write即可，不必writen
            }
            if (kev[i].ident == fileno(fp)) {
                n = read(fileno(fp), buf, MAXBUFSIZE);
                if (n > 0)
                    writen(sockfd, buf, n);
                if (n == 0 || (isfile && n == kev[i].data)) {
                    stdineof = 1;
                    shutdown(sockfd, SHUT_WR);
                    //3、注册kevent，DELETE事件
                    kev[i].flags = EV_DELETE;            
                    kevent(kq, &kev[i], 1, NULL, 0, &ts);//(&kev[i], 1)合起来，表示的也就是key[i]本身
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