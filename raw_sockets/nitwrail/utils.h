#ifndef _UTILS_H
#define _UTILS_H

int tcpsock_bind(char * host, unsigned int port, unsigned short backlog);
int tcpsock_connect(char * host, unsigned int port);
int tcpsock_accept(int sfd);
int udpsock_bind(char * host, unsigned int port);
int send_packet(int rsfd, void * buffer, unsigned int length);
int recv_packet(int rsfd, void * buffer, unsigned int length);
int unixsock_bind(const char * pathname);
int unixsock_accept(int usfd);
int unixsock_connect(const char * pathname);
int send_fd(int socket, int fd);
int recv_fd(int socket);

#endif // _UTILS_H
