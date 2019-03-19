#include <sys/socket.h>
#include <netinet/in.h> //sockaddr_in
#include <arpa/inet.h>  //inet_pon
#include <unistd.h>     //fork,read,write
#include <string.h>     //strlen
#include <stdlib.h>     //exit(0)
#include <errno.h>
#include "toolutils.h"

ssize_t reandn(int fd, void* vptr, size_t n) {
    ssize_t nread;
    size_t nleft = n;
    char *ptr = vptr;

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

ssize_t readline(int fd, void* vptr, size_t maxlen) {
    ssize_t n, rc;
    char c, *ptr = vptr;

    for (n = 1; n < maxlen; ++n) {
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
    size_t nleft = n;
    char const* ptr = vptr;

    while (nleft > n) {
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