#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <netdb.h>

/**
 * This function binds a specific IPv4 and Port and Listen for requests with
 * backlog given as parameter
 * @param host is an IPv4 address in char array format
 * @param port is an unsigned integer of range 1024-65535
 * @param backlog is unsigned short of range 1-128 is used for
 * no. of client waits in case of concurrent connection requests
 * @return integer socket file descriptor, on success it return > 0 otherwise -1
 */
int tcpsock_bind(char * host,unsigned int port, unsigned short backlog) {
	struct sockaddr_in addr; int sfd;
	sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sfd < 0) {
		perror("socket() ");
		return -1;
	}
	memset((char *)&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host);
	if(bind(sfd, (struct sockaddr*)&addr, sizeof addr) < 0) {
		perror("bind() ");
		close(sfd);
		return -1;
	}
	if(listen(sfd, backlog) < 0) {
		perror("listen() ");
		close(sfd);
		return -1;
	}
	return sfd;
}

/**
 * This function creates a tcp socket and connects with IP:Port given as parameter
 * @param  host is an IPv4 address in char array format
 * @param  port port is an unsigned integer of range 1024-65535
 * @return      integer socket file descriptor, on success it return > 0 otherwise -1
 */
int tcpsock_connect(char * host, unsigned int port) {
	struct sockaddr_in addr; int sfd;
	sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sfd < 0) {
		perror("socket() ");
		return -1;
	}
	memset((char *)&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host);

	if(connect(sfd, (struct sockaddr*)&addr, sizeof addr) < 0) {
		perror("connect() ");
		close(sfd);
		return -1;
	}
	return sfd;
}

/**
 * Accepts TCP connection on a socket file descriptor
 * @param  sfd TCP binded socket file desciptor
 * @return     socket file descriptor for client, > 0 if success otherwise -1
 */
int tcpsock_accept(int sfd) {
	struct sockaddr_in addr; int cli_len = sizeof addr;
	int nsfd;
	nsfd = accept(sfd, (struct sockaddr*)&addr, &cli_len);
	if(nsfd < 0) {
		perror("accept() ");
		return -1;
	}
	return nsfd;
}
/**
 * @param host is an IPv4 address in char array format
 * @param port is an unsigned integer of range 1024-65535
 * @return integer socket file descriptor, on success it return > 0 otherwise -1
 */
int udpsock_bind(char * host, unsigned int port){
    struct sockaddr_in addr; int sfd;
	sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sfd < 0) {
		perror("socket() ");
		return -1;
	}
	memset((char *)&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host);
	if(bind(sfd, (struct sockaddr*)&addr, sizeof addr) < 0) {
		perror("bind() ");
		close(sfd);
		return -1;
	}
    return sfd;
}
/**
 * Creates a unix socket and connects with socket at given pathname
 * @param pathname The pathname which is binded by other unix socket
 * @return unix socket file desciptor, on success > 0 else -1
 */
int unixsock_connect(const char * pathname) {
	struct sockaddr_un addr;
	int len, usfd;
	if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		perror("socket() ");
		return -1;
	}
	addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, pathname);
    len = strlen(addr.sun_path) + sizeof(addr.sun_family);
    if (connect(usfd, (struct sockaddr *)&addr, len) == -1) {
        perror("connect() ");
        return -1;
    }
    return usfd;
}
/**
 * @param pathname
 * @return
 */
int unixsock_bind(const char * pathname) {
    struct sockaddr_un addr;
    int len, usfd;
    if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("socket() ");
        return -1;
    }
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, pathname);
    len = strlen(addr.sun_path) + sizeof(addr.sun_family);
    if (bind(usfd, (struct sockaddr *)&addr, len) == -1) {
        perror("bind() ");
        return -1;
    }
    if(listen(usfd, 5) < 0) {
        perror("listen() ");
        return -1;
    }
    return usfd;
}

/**
 * @param usfd
 * @return
 */
int unixsock_accept(int usfd) {
    int nsfd, len; struct sockaddr_un addr;
    len = sizeof addr;
    nsfd = accept(usfd, (struct sockaddr*)&addr, &len);
    return nsfd;
}
/**
* send file desciptor over unix sockets from given socket
* @param socket socket file desciptor on which it sends fd
* @return integer, > 0 on success else -1 on error.
 */
int send_fd(int socket, int fd) {
	struct msghdr message; struct iovec iov[1];
	struct cmsghdr *ctrl_msg = NULL;
	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];

	memset(&message, 0, sizeof(struct msghdr));
	memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

	/* We are passing at least one byte of data so that recvmsg() will not return 0 */
	data[0] = '*';
	iov[0].iov_base = data;
	iov[0].iov_len = sizeof(data);

	message.msg_name = NULL;
	message.msg_namelen = 0;
	message.msg_iov = iov;
	message.msg_iovlen = 1;
	message.msg_controllen =  CMSG_SPACE(sizeof(int));
	message.msg_control = ctrl_buf;

	ctrl_msg = CMSG_FIRSTHDR(&message);
	ctrl_msg->cmsg_level = SOL_SOCKET;
	ctrl_msg->cmsg_type = SCM_RIGHTS;
	ctrl_msg->cmsg_len = CMSG_LEN(sizeof(int));

	int * fdptr = (int *) CMSG_DATA(ctrl_msg);
    memcpy((void *)fdptr, (void *)&fd, sizeof(int));

	return sendmsg(socket, &message, 0);
}
/**
 * Recieves file desciptor over unix sockets from given socket
 * @param socket socket file desciptor on which it recieves fd
 * @return integer file desciptor, > 0 on success else -1 on error.
 */
int recv_fd(int socket) {
 	struct msghdr message;
 	struct iovec iov[1];
 	struct cmsghdr *ctrl_msg = NULL;
 	char ctrl_buf[CMSG_SPACE(sizeof(int))];
 	char data[1]; int fd;

    memset(&message, 0, sizeof(struct msghdr));
    memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

    /* For the dummy data */
    iov[0].iov_base = data;
    iov[0].iov_len = sizeof(data);

	message.msg_name = NULL;
    message.msg_namelen = 0;
    message.msg_control = ctrl_buf;
    message.msg_controllen = CMSG_SPACE(sizeof(int));
    message.msg_iov = iov;
    message.msg_iovlen = 1;

    if(recvmsg(socket, &message, 0) < 0)
    	return -1;
  	ctrl_msg = CMSG_FIRSTHDR(&message);
  	if((ctrl_msg->cmsg_level == SOL_SOCKET) && (ctrl_msg->cmsg_type == SCM_RIGHTS)) {
  		int * _fd = (int *) CMSG_DATA(ctrl_msg);
  		fd = * _fd;
  	}
  	return fd;
}

/* End of utils.c */
