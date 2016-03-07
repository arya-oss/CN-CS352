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

int shmid1, shmid, id;
int * aval; char * shm;
char buf[128];

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

void handler() {
	*(aval+id) = 1; // make it busy
	printf("%s\n", shm);
	int fd = open(shm, O_RDONLY);
	if(fd == -1) eerror("open() error");
	read(fd, buf, 128);
	printf("Platform %d Display Sequence is: \n\t %s\n", id+1, buf);
	sleep(5);
	close(fd);
	*(aval+id) = 0; // make it available
}

int main(int argc, char const *argv[])
{
	signal(SIGUSR1, handler);
	if(argc < 2) 
		eerror("Usage: \n\t ./platform [1|2|3]\n");
	id = atoi(argv[1])-1;
	sharedMem();
	while(1) {
		sleep(0.5);
	}
	return 0;
}
