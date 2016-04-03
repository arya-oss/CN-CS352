#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/ipc.h>

#define SHMKEY 0x123
#define SHMKEY1 0x124
#define eerror(msg) { printf("%s\n", msg); exit(1); }

int fd_r=-1, fd_w=-1;
int shmid1, shmid;
char tmp[32],buf[32], buf1[512];
char * sbuf;

void handler(int signo){
	//printf("%s\n", sbuf);
	sprintf(tmp, "%sr.dat", sbuf);
	fd_w = open(tmp, O_WRONLY);
	if(fd_w == -1) 
		error("WERROR");
	sprintf(tmp, "%sw.dat", sbuf);
	fd_r = open(tmp, O_RDONLY);
	if(fd_r == -1)
		error("RERROR");
	printf("%s opened\n", sbuf);
	signal(SIGUSR1, handler);
}

int main(int argc, char * argv[]) {
	if(argc == 1) {
		printf("usage: ./service <serv_no>\n");
		exit(1);
	}
	signal(SIGUSR1, handler);	
	shmid = shmget(SHMKEY, 40, 0666|IPC_CREAT);
    if(shmid == -1) {
        eerror("shmget() error");
    }
    sbuf = (char *)shmat(shmid, NULL, 0);
    if(sbuf == (char *)-1) {
        eerror("shmat() error");
    }
    shmid1 = shmget(SHMKEY1, 40, 0666|IPC_CREAT);
    if(shmid1 == -1) {
        eerror("shmget() error");
    }
    int * serv_id = (int *)shmat(shmid1, NULL, 0);
    if(serv_id == (int *)-1) {
        eerror("shmat() error");
    }
    
    int id = atoi(argv[1])-1;
    *(serv_id+5+id) = 0;
    *(serv_id+id) = (int)getpid();
    printf("starting service %s\n", argv[1]);
	while(1){
		//printf("%d %d\n", fd_r, fd_w);
		if(fd_r == -1){
			sleep(1);
			continue;
		}
		if(read(fd_r, buf, 32) <= 0) {
			sleep(0.5);
		} else {
			printf("%s\n", buf);
			if(strcmp(buf, "exit") == 0){
				printf("client disconnected\n");
				close(fd_w);
				close(fd_r);
				fd_r = fd_w = -1;
				*(serv_id+5+id) = 0;
				continue;
			}
			FILE * f = popen(buf, "r");
			while(read(fileno(f), buf1, 512) > 0) {
				write(fd_w, buf1, 512);
			}
			pclose(f);
		}
	}
	return 0;
}