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
	if(argc < 3) {
		printf("Usage: ./server <temperory_sock_path> <target_file_path>\n");
		exit(1);
	}
	unlink(argv[1]); // if sock_path exists remove it.
	int fd = open(argv[2], O_RDONLY);
	struct sockaddr_un remote; int len,n; char buf[BUFSIZE];
	if(fd < 0){
		perror("open() ");
		exit(1);
	}
	int usfd = init_sockbind(argv[1]);
	if(usfd < 0){
		printf("sock() error\n");
		exit(1);
	}
	len = sizeof(remote);
	int nusfd = accept(usfd, (struct sockaddr*)&remote, &len);	
	if(nusfd < 0) {
		perror("accept() ");
		exit(1);
	}
	while(1) {
		printf("Number of bytes to print: ");
		scanf("%d", &n);
		read(fd, buf, n);
		printf("%s\n", buf);
		memset(buf, 0, n);
		if(send_fd(nusfd, fd) < 0){
			perror("send_fd() ");
			exit(1);
		}
		close(fd);
		fd = recv_fd(nusfd);
		if(fd < 0) {
			perror("recv_fd() ");
			exit(1);
		}
	}
	return 0;
}