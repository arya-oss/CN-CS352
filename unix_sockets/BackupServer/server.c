#include "utils.h"

#define BUFSIZE 1024
#define MAXF 2

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
		for(j=0; j<nsfd_cnt; j++) {
			if(nsfds[j] == -1) continue;
			FD_SET(nsfds[j], &rfds);
			if(max < nsfds[j])
				max = nsfds[j];
		}
		MAXC = nsfd_cnt;
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

int main(int argc, char const *argv[]) {
	int usfd, nusfd, nsfd, sfd, i, j, len;
	pthread_t pid;
	pthread_mutex_init(&lock, NULL);
	usfd = init_sockbind("/tmp/sockpath");
	if(usfd < 0) {
		perror("init_sockbind() ");
		exit(1);
	}
	sfd = tcpsocket_bind(8001);
	if(sfd < 0) {
		perror("tcpsocket_bind() ");
		exit(2);
	}
	nusfd = sock_accept(usfd);
	if(nusfd < 0) {
		perror("sock_accept() ");
		exit(3);
	}

	struct ucred cred;
	len = sizeof(struct ucred);
	
	if(getsockopt(nusfd, SOL_SOCKET, SO_PEERCRED, &cred, &len) < 0) {
		perror("getsockopt() ");
		exit(4);
	}
	// printf("%ld\n", (long) cred.pid); // backup server pid
	pthread_create(&pid, NULL, &runner, NULL);
	while(1) {
		nsfd = tcp_accept(sfd);
		if(nsfd < 0) {
			perror("tcp_accept() ");
			continue;
		}
		if(nsfd_cnt == MAXF) {
			for (i = 0; i < MAXF; ++i) {
				pthread_mutex_lock(&lock);
				if(send_fd(nusfd, nsfds[i]) < 0) {
					perror("send_fd() ");
				}
				pthread_mutex_unlock(&lock);
				// close(nsfds[i]);
			}
			nsfd_cnt = 0;
			sleep(2);
			kill(cred.pid, SIGUSR1);
		}
		pthread_mutex_lock(&lock);
		nsfds[nsfd_cnt++] = nsfd;
		pthread_mutex_unlock(&lock);
	}
	return 0;
}
