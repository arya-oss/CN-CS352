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

#include "utils.h"
#define BUFSIZE 1024

int main(int argc, char const *argv[])
{
	if(argc < 2) {
		printf("Usage: ./server <temperory_sock_path>\n");
		exit(1);
	}
	int usfd = init_sockconnect(argv[1]);
	if(usfd < 0){
		printf("sock() error\n");
		exit(1);
	}
	int fd,n; char buf[BUFSIZE];

	printf("Process 2 Started !!\n");
	while(1) {
		fd = recv_fd(usfd);
		if(fd < 0) {
			perror("recv_fd() ");
			exit(1);
		}
		printf("Number of bytes to print: ");
		scanf("%d", &n);
		read(fd, buf, n);
		printf("%s\n", buf);
		memset(buf, 0, n);
		if(send_fd(usfd, fd) < 0){
			perror("send_fd() ");
			exit(1);
		}
		close(fd);
	}
	return 0;
}