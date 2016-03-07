#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include <unistd.h>
#include <poll.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }

int pids[3];
int shmid, shmid1;
int * aval; char * shm;

void sharedMem() {
	shmid = shmget(0x123, 12, IPC_CREAT|0666);
	if(shmid == -1) eerror("shmget() error");
	aval = (int *)shmat(shmid, NULL, 0);
	if(aval == (int *)-1)
		eerror("shmat() error");
	shmid1 = shmget(0x124, 128, IPC_CREAT|0666);
	if(shmid == -1) eerror("shmget() error");
	shm = (char *)shmat(shmid1, NULL, 0);
	if(shm == (char *)-1)
		eerror("shmat() error");
}

void platformInit() {
	int pid;
	pid = fork();
	if(pid == 0) {
		if(execl("platform", "platform", "1", NULL) == -1)
			eerror("execl() error")
	} else {
		pids[0] = pid; *(aval) = 0;
	}
	pid = fork();
	if(pid == 0) {
		if(execl("platform", "platform", "2", NULL) == -1)
			eerror("execl() error")
	} else {
		pids[1] = pid; *(aval+1) = 0;
	}
	pid = fork();
	if(pid == 0) {
		if(execl("platform", "platform", "3", NULL) == -1)
			eerror("execl() error")
	} else {
		pids[2] = pid; *(aval+2) = 0;
	}
}

int main() {
	sharedMem();
	struct pollfd ends[4]; int i, j, ret; char buf[128], tmp[128];
	ends[0].fd = open("h_fifo", O_RDWR);
	ends[1].fd = open("d_fifo", O_RDWR);
	ends[2].fd = open("v_fifo", O_RDWR);
	ends[3].fd = open("a_fifo", O_RDWR);
	for (i = 0; i < 4; ++i){
		if(ends[i].fd == -1)
			eerror("open() error");
		ends[i].events = POLLIN;
	}
	platformInit();
	printf("Station started listening ...\n");
	while(1) {
		ret = poll(ends, 4, 100);
		if(ret < 0) {
			sleep(0.5);
			continue;
		}
		for (i = 0; i < 3; ++i) {
			if(ends[i].revents & POLLIN) {
				memset(buf, 0, 128);
				read(ends[i].fd, buf, 128);
				int flag=1;
				for(j=0; j<3; j++) {
					if(*(aval+j) == 0) {
						flag = 0;
						sprintf(shm, "%s", buf);
						int fd = open(buf, O_WRONLY);
						sprintf(buf, "%d", j+1);
						write(fd, buf, strlen(buf));
						close(fd);
						kill(pids[j], SIGUSR1);
						break;
					}
				}
				if(flag==1) {
					printf("not available\n");
					int fd = open(buf, O_WRONLY);
					write(fd, "exit", 4);
					close(fd);
				}
			}
		}
		if (ends[3].revents & POLLIN){
			read(ends[i].fd, buf, 128);
			printf("Acknowledgement of %s\n", buf);
		}
	}
	return 0;
}
