#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <error.h>

int init_sockbind(const char * path);
int init_sockconnect(const char * path);
int send_fd(int sock_fd, int fd);
int recv_fd(int sock_fd);

#endif // _UTILS_H