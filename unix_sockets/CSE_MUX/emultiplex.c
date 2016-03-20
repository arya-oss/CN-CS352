#include "utils.h"

#define BUFSIZE 1024
#define MAXF 1

volatile int nsfds[MAXF], nsfd_cnt=0;
pthread_mutex_t lock;

void * runner(void * args) {
	int max,MAXC,i,j; char buf[BUFSIZE];
	printf("start servicing\n"); struct timeval tv;
	fd_set rfds;
	while(1) {
		FD_ZERO(&rfds);
		pthread_mutex_lock(&lock);
		max = -1;
		MAXC = nsfd_cnt;
		for(j=0; j<nsfd_cnt; j++) {
			FD_SET(nsfds[j], &rfds);
			if(max < nsfds[j])
				max = nsfds[j];
		}
		pthread_mutex_unlock(&lock);
		tv.tv_sec = 1; tv.tv_usec = 0;
		int res = select(max+1, &rfds, NULL, NULL, &tv);
		if(res < 0) {
			printf("select() ");
			continue;
		}
		// printf("ok %d\n", MAXC);
		for(i=0; i<MAXC; i++) {
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

int main(int argc, char const *argv[]) {
	pthread_t pid;
	pthread_mutex_init(&lock, NULL);
	int usfd, nusfd, sfd, n; char buf[BUFSIZE];
	usfd = init_sockbind("/tmp/sockpath3");
	if(usfd < 0) {
		perror("init_sockbind() ");
		exit(1);
	}
	pthread_create(&pid, NULL, &runner, NULL);
	nusfd = sock_accept(usfd);
	if(nusfd < 0) {
		perror("sock_accept() ");
		exit(1);
	}
	while(1) {
		int nsfd = recv_fd(nusfd);
		if(nsfd < 0) {
			perror("recv_fd() ");
			continue;
		}
		if(nsfd_cnt == MAXF) {
			write(nsfd, "-1", 2);
			close(nsfd);
			int _tmp_usfd = init_sockconnect("/tmp/sockpath0");
			if(_tmp_usfd < 0) {
				perror("init_sockconnect() ");
				continue;
			}
			struct ucred cred;
			int len = sizeof(cred);
			if(getsockopt(_tmp_usfd, SOL_SOCKET, SO_PEERCRED, &cred, &len) < 0) {
				perror("getsockopt() ");
			}
			kill(cred.pid, SIGUSR1);
			close(_tmp_usfd);
			printf("signaled to Ticket Counter !\n");
			continue;
		}
		// pthread_mutex_lock(&lock);
		write(nsfd, "E Multiplex", 11);
		nsfds[nsfd_cnt++] = nsfd;
		// pthread_mutex_unlock(&lock);
		printf("client added !\n");
	}
	return 0;
}
