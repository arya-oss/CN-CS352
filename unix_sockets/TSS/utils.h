#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/select.h>
#include <fcntl.h>
#include <pthread.h>
#include <error.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <sys/un.h>
#include <error.h>

int init_sockbind(const char * path);
int init_sockconnect(const char * path);
int sock_accept(int sfd);
int send_fd(int sock_fd, int fd);
int recv_fd(int sock_fd);
int tcpsocket_bind(int port);
int tcpsocket_connect(int port);

#endif // _UTILS_H