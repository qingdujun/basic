#ifndef MOSS_NETS_TCP_UNIX_TOOLUTILS_H
#define MOSS_NETS_TCP_UNIX_TOOLUTILS_H

ssize_t reandn(int fd, void* vptr, size_t n);
ssize_t readline(int fd, void* vptr, size_t maxlen);
ssize_t writen(int fd, void const* vptr, size_t n);

#endif