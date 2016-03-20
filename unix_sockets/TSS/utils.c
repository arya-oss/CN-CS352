#include "utils.h"
/**
 * Author: Rajmani Arya
 * Immported from Repo of https://github.com/kranthikiran01/Computer-Networks
 * @param  PATH unique temprory path for passing fds
 * @return socket fd on success or return -1 on error !
 */
int init_sockconnect(const char * PATH) {
	struct sockaddr_un addr;
	int len, usfd;
	if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		perror("socket() ");
		return -1;
	}
	addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, PATH);
    len = strlen(addr.sun_path) + sizeof(addr.sun_family);
    if (connect(usfd, (struct sockaddr *)&addr, len) == -1) {
        perror("connect() ");
        return -1;
    }
    return usfd;
}

int init_sockbind(const char * PATH) {
    struct sockaddr_un addr;
    int len, usfd;
    if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("socket() ");
        return -1;
    }
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, PATH);
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

int send_fd(int sock_fd, int fd) {
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

	return sendmsg(sock_fd, &message, 0);
}

int recv_fd(int sock_fd) {
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

    if(recvmsg(sock_fd, &message, 0) < 0)
    	return -1;
  	ctrl_msg = CMSG_FIRSTHDR(&message);
  	if((ctrl_msg->cmsg_level == SOL_SOCKET) && (ctrl_msg->cmsg_type == SCM_RIGHTS)) {
  		int * _fd = (int *) CMSG_DATA(ctrl_msg);
  		fd = * _fd;
  	}
  	return fd;
}

int tcpsocket_bind(int port) {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1) {
        perror("socket() ");
        return -1;
    }
    struct sockaddr_in addr;
    memset((char *)&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(bind(sfd, (struct sockaddr*)&addr, sizeof addr) < 0){
        perror("bind() ");
        close(sfd);
        return -1;
    }
    if(listen(sfd, 5) < 0) {
        perror("listen() ");
        close(sfd);
        return -1;
    }
    return sfd;
}

int tcpsocket_connect(int port) {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1) {
        perror("socket() ");
        return -1;
    }
    struct sockaddr_in addr;
    memset((char *)&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(sfd, (struct sockaddr*)&addr, sizeof addr) < 0){
        perror("connect() ");
        close(sfd);
        return -1;
    }
    return sfd;
}

int sock_accept(int sfd) {
    int nsfd, len; struct sockaddr_un addr;
    len = sizeof addr;
    nsfd = accept(sfd, (struct sockaddr*)&addr, &len);  
    return nsfd;
}