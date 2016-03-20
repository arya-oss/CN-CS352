#include "utils.h"

#define BUFSIZE 1024
#define MAXF 32

volatile int nsfds[MAXF], fd_cnt=0;
pthread_mutex_t lock;

void * runner(void * args) {
	int max,MAXC,i,j; char buf[BUFSIZE];
	printf("start servicing\n"); struct timeval tv;
	fd_set rfds;
	while(1) {
		FD_ZERO(&rfds);
		// pthread_mutex_lock(&lock);
		max = -1;
		MAXC = fd_cnt;
		for(j=0; j<fd_cnt; j++) {
			if(nsfds[j] == -1) continue;
			FD_SET(nsfds[j], &rfds);
			if(max < nsfds[j])
				max = nsfds[j];
		}
		// pthread_mutex_unlock(&lock);
		tv.tv_sec = 1; tv.tv_usec = 0;
		int res = select(max+1, &rfds, NULL, NULL, &tv);
		if(res < 0) {
			printf("select() ");
			continue;
		}
		// printf("%d\n", MAXC);
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
				for(j=0; j<strlen(buf); j++) {
					if(buf[j] >= 'a' && buf[j] <= 'z')
					buf[j] -= 32;
				}
				write(nsfds[i], buf, strlen(buf));
				memset(buf, 0, BUFSIZE);
			}
		}
	}
}

void handler(int sig) {
	pthread_t pid;
	printf("signal recieved !!\n");
	pthread_create(&pid, NULL, &runner, NULL);
}

int main(int argc, char const *argv[]) {
	signal(SIGUSR1, handler);
	pthread_mutex_init(&lock, NULL);
	int usfd = init_sockconnect("/tmp/sockpath");
	/* int optval = 1;
    if (setsockopt(usfd, SOL_SOCKET, SO_PASSCRED, &optval, sizeof(optval)) == -1)
        perror("setsockopt() ");
	*/
	if(usfd < 0){
		perror("sock_connect() ");
		exit(1);
	}
	while(1) {
		pthread_mutex_lock(&lock);
		nsfds[fd_cnt++] = recv_fd(usfd);
		pthread_mutex_unlock(&lock);
		if(nsfds[fd_cnt-1] < 0) {
			perror("recv_fd() ");
		}
		printf("nsfd recieved %d!!\n", nsfds[fd_cnt-1]);
	}
	return 0;
}
