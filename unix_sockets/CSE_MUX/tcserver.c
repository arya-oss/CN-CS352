#include "utils.h"

#define BUFSIZE 1024
#define MAXC 3

volatile int sfds[MAXC], seatfull = 0, tkt_no=10001;

void * runner(void * args) {
	pthread_mutex_t lock; pthread_mutex_init(&lock, NULL);
	int sfd, nusfd, usfd = init_sockbind("/tmp/sockpath0");
	nusfd = sock_accept(usfd);
	if(nusfd < 0) {
		perror("sock_accept() ");
	}
	close(usfd);
	close(nusfd);
}

void * listener(void * args) {
	int max,i,j, nsfd, usfd = -1; char buf[BUFSIZE];
	printf("Ticket Counter Started !\n"); struct timeval tv;
	fd_set rfds;
	while(1) {
		FD_ZERO(&rfds);
		max = -1;
		for(j=0; j<MAXC; j++) {
			FD_SET(sfds[j], &rfds);
			if(max < sfds[j])
				max = sfds[j];
		}
		tv.tv_sec = 1; tv.tv_usec = 0;
		int res = select(max+1, &rfds, NULL, NULL, &tv);
		if(res < 0) {
			printf("select() ");
			continue;
		}
		for(i=0; i<MAXC; i++) {
			if(FD_ISSET(sfds[i], &rfds)){
				nsfd = tcp_accept(sfds[i]);
				if(seatfull) {
					printf("seatfull\n");
					write(nsfd, "-1", 2);
					close(nsfd);
				} else {
					write(nsfd, "Which Multiplex (1,2,3) ? ", 28);
					read(nsfd, buf, BUFSIZE);
					int id = atoi(buf);
					printf("Requested for Multiplex %d\n", id);
					sprintf(buf, "%d", tkt_no++);
					write(nsfd, buf, strlen(buf));
					if(usfd < 0) {
						strcpy(buf, "/tmp/sockpath1");
						usfd = init_sockconnect(buf);
						if(usfd < 0) {
							perror("init_sockconnect() ");
						}
					}
					if(usfd > 0) {
						if(send_fd(usfd, nsfd) < 0) {
							perror("send_fd() ");
						}
					}
				}
			}
		}
	}
}

void handler(int sig) {
	printf("signal from last Multiplex\n");
	seatfull = 1;
}

int main(int argc, char const *argv[]) {
	signal(SIGUSR1, handler);
	int i, ports[] = {8001, 8002, 8003};
	for (i = 0; i < MAXC; ++i) {
		sfds[i] = tcpsocket_bind(ports[i]);
		if(sfds[i] < 0) {
			perror("tcpsocket_bind() ");
			exit(1);
		}
		printf("Listening on %d\n", ports[i]);
	}
	pthread_t pid1, pid2;
	pthread_create(&pid1, NULL, &runner, NULL);
	pthread_create(&pid2, NULL, &listener, NULL);
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	return 0;
}
