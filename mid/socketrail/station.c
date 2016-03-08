#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include <unistd.h>
#include <poll.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }

int pids[3], shmid, shmid1;
int * aval; char * shm;

void sharedMem() {
	shmid = shmget(0x124, 128, IPC_CREAT|0666);
	if(shmid == -1) eerror("shmget() error");
	aval = (int *)shmat(shmid, NULL, 0);
	if(aval == (int *)-1)
		eerror("shmat() error");
	shmid1 = shmget(0x123, 128, IPC_CREAT|0666);
	if(shmid1 == -1) eerror("shmget() error");
	shm = (char *)shmat(shmid1, NULL, 0);
	if(shm == (char *)-1)
		eerror("shmat() error");
}

void platformInit() {
	int pid;
	pid = fork();
	if(pid == 0) {
		if(execl("platform", "platform", "1", "8004", NULL) == -1)
			eerror("execl() error")
	} else {
		pids[0] = pid; *(aval) = 0;
	}
	pid = fork();
	if(pid == 0) {
		if(execl("platform", "platform", "2", "8005", NULL) == -1)
			eerror("execl() error")
	} else {
		pids[1] = pid; *(aval+1) = 0;
	}
	pid = fork();
	if(pid == 0) {
		if(execl("platform", "platform", "3", "8006", NULL) == -1)
			eerror("execl() error")
	} else {
		pids[2] = pid; *(aval+2) = 0;
	}
}

void sig_handler() {
	printf("signal caught\n");
	printf("%s\n", shm);
}

int main() {
	sharedMem();
	signal(SIGUSR1, sig_handler);

	int ports[] = { 8001, 8002, 8003 };
	struct pollfd ends[3]; int i, sfd, nsfd, j, ret, cli_len; char buf[128], tmp[128]; 
	struct sockaddr_in s_addr, c_addr;
	for (i = 0; i < 3; ++i) {
		sfd = socket(AF_INET, SOCK_STREAM, 0);
		if(sfd == -1) eerror("socket() error");
		memset((char *)&s_addr, 0, sizeof(s_addr));

	    s_addr.sin_family = AF_INET;
	    s_addr.sin_port = htons((u_short) ports[i]);
	    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	    if( bind(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
	        eerror("bind() error");

	    if(listen(sfd, 5) < 0)
	        eerror("listen() error");
	    printf("%d Ends listening ..\n", ports[i]);
	    ends[i].fd = sfd;
	    ends[i].events = POLLIN;
	}

	platformInit();
	printf("Station started listening ...\n");
	while(1) {
		ret = poll(ends, 3, 100);
		if(ret < 0) {
			sleep(0.5);
			continue;
		}
		for (i = 0; i < 3; ++i) {
			if(ends[i].revents & POLLIN) {
				// printf("Req on %d\n", i);
				memset((char *) &c_addr, 0, sizeof(c_addr));
				cli_len = sizeof(c_addr);
				if((nsfd=accept(ends[i].fd, (struct sockaddr *)&c_addr, &cli_len)) < 0)
                    eerror("accept() error");
				int flag=1;
				memset(buf, 0, 128);
				sprintf(buf, "-1");
				for(j=0; j<3; j++) {
					if(*(aval+j) == 0) {
						flag = 0;
						sprintf(buf, "%d", ports[j]+3); // write platform port from 8004 
						write(nsfd, buf, strlen(buf));
						sleep(0.5);
						kill(pids[j], SIGUSR1);
						break;
					}
				}
				if(flag==1) {
					write(nsfd, buf, strlen(buf));
				}
				close(nsfd);
			}
		}
	}
	return 0;
}
