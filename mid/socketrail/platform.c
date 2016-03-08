#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#include <unistd.h>
#include <poll.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }

int shmid, shmid1, port, sfd, id;
int * aval; char * shm;
char buf[128];

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


void handler() {
	*(aval+id) = 1; // make it busy
	struct sockaddr_in c_addr; int cli_len;
	// printf("In the handler\n");
	int nsfd = accept(sfd, (struct sockaddr *)&c_addr, &cli_len);
	if(nsfd == -1) eerror("accept() error");
	while(1) {
		read(nsfd, buf, 128);
		if(atoi(buf) == -1)
			break;
		printf("Platform %d Display Sequence is: \n\t %s\n", port, buf);
	}
	sleep(3);
	sprintf(shm, "platform %d train left", id+1);
	kill(getppid(), SIGUSR1);
	close(nsfd);
	*(aval+id) = 0; // make it available
}

int main(int argc, char const *argv[])
{
	signal(SIGUSR1, handler);
	if(argc < 3) 
		eerror("Usage: \n\t ./platform [1|2|3] <port>\n");
	port = atoi(argv[2]);
	id = atoi(argv[1])-1;

	sharedMem();
	struct sockaddr_in s_addr;

    memset((char *) &s_addr, 0, sizeof(s_addr));
    if ((sfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
        eerror("socket() error");

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons((u_short) port);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
        eerror("bind() error");

    if(listen(sfd, 5) < 0)
        eerror("listen() error");

	while(1) {
		sleep(0.5);
	}
	return 0;
}
