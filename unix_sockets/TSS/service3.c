#include "utils.h"
#include <sys/time.h>
#define BUFSIZE 1024
/**
 * Service for UPPERCASE Server
 */

int nsfds[32], nClient=0; 
pthread_mutex_t lock;

void * runner(void * args) {
	int max,MAXC,i,j; char buf[BUFSIZE];
	printf("start servicing\n"); struct timeval tv;
	fd_set rfds;
	while(1) {
		FD_ZERO(&rfds);
		pthread_mutex_lock(&lock);
		max = -1;
		for(j=0; j<nClient; j++) {
			if(nsfds[j] == -1) continue;
			FD_SET(nsfds[j], &rfds);
			if(max < nsfds[j])
				max = nsfds[j];
		}
		MAXC = nClient;
		pthread_mutex_unlock(&lock);
		tv.tv_sec = 1; tv.tv_usec = 0;
		int res = select(max+1, &rfds, NULL, NULL, &tv);
		if(res < 0) {
			printf("select() ");
			continue;
		}
		for(i=0; i<MAXC; i++) {
			if(nsfds[i] == -1) continue;
			if(FD_ISSET(nsfds[i], &rfds)){
				read(nsfds[i], buf, BUFSIZE);
				printf("%s\n", buf);
				if(atoi(buf) == -1){
					close(nsfds[i]);
					nsfds[i] = -1;
					printf("Client Exitting..\n");
					continue;
				}

				write(nsfds[i], buf, strlen(buf));
				memset(buf, 0, BUFSIZE);
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	pthread_mutex_init(&lock, NULL);
	if(argc < 2) {
		printf("Usage: ./service <temperory_sock_path>\n");
		exit(1);
	}
	int usfd = init_sockconnect(argv[1]);
	if(usfd < 0){
		printf("sock() error\n");
		exit(1);
	}
	printf("service 3 started !!\n");
	int fd,n; pthread_t pid;
	fd_set rfds;
	pthread_create(&pid, NULL, &runner, NULL);
	while(1) {
		fd = recv_fd(usfd);
		if(fd < 0) {
			perror("recv_fd() ");
			continue;
		}
		pthread_mutex_lock(&lock);
		nsfds[nClient++] = fd;
		pthread_mutex_unlock(&lock);
		printf("Client added !\n");
	}
	return 0;
}