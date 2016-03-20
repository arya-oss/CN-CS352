#include "utils.h"
#define BUFSIZE 1024
/**
 * Service for LOWERCASE Server
 */
void * runner(void * args) {
	int i,fd = *(int *)args; char buf[BUFSIZE];
	printf("start servicing\n");
	while(1) {
		read(fd, buf, BUFSIZE);
		// printf("%s\n", buf);
		if(atoi(buf) == -1){
			close(fd);
			printf("Client Exitting..\n");
			pthread_exit(0);
		}
		write(fd, buf, strlen(buf));
		memset(buf, 0, BUFSIZE);
	}
}

int main(int argc, char const *argv[])
{
	if(argc < 2) {
		printf("Usage: ./service <temperory_sock_path>\n");
		exit(1);
	}
	int usfd = init_sockconnect(argv[1]);
	if(usfd < 0){
		printf("sock() error\n");
		exit(1);
	}
	printf("service started !!\n");
	int fd,n; pthread_t pid;
	fd_set rfds;
	while(1) {
		fd = recv_fd(usfd);
		if(fd < 0) {
			perror("recv_fd() ");
			continue;
		}
		pthread_create(&pid, NULL, runner, (void *) &fd);
	}
	return 0;
}