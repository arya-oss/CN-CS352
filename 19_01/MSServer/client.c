#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }
#define sfifo "c2s_fifo.dat"

int fd_w, fd_r; char buf[512], tmp[128];

void writer(void * args) {
	while(1){
        memset(tmp, 128, '\0');
        scanf("%s", tmp);
        if(strcmp(tmp, "exit") == 0)
        	exit(127);
        write(fd_w, tmp, 128);
    }
}

void reader(void * args) {
	while(1){
        memset(buf, 512, '\0');
        read(fd_r, buf, 512);
        printf("%s", buf);
    }
}

void handler(int signo) {
	write(fd_w, "exit", 128);
	printf("\nExiting . . .\n");
	exit(1);
}

int main (int argc, char * argv[]){
    if(argc < 3)
        eerror("Usage: ./client <service-name> <fifo-prefix>");
    signal(SIGINT, handler);
    pthread_t pid1, pid2;

	sprintf(tmp, "%sr.dat", argv[2]);
    mkfifo(tmp, 0666);

    sprintf(buf, "%sw.dat", argv[2]);
    mkfifo(buf, 0666);
    
    sprintf(buf, "%s %s", argv[1], argv[2]);
   
    int fd = open(sfifo, O_WRONLY);
    write(fd, buf, strlen(buf));  // request to server for connection
    close(fd);
    
    sprintf(buf, "%sw.dat", argv[2]);
    fd_r = open(tmp, O_RDONLY);
    if(fd_r == -1) eerror("fd_r error");
    
    fd_w = open(buf, O_WRONLY);
    if(fd_w == -1) eerror("fd_w error");

    int res = pthread_create(&pid1, NULL, reader, NULL);
    if (res == -1) 
    	eerror("pthread_create() error");

    res = pthread_create(&pid2, NULL, writer, NULL);
    if (res == -1) 
    	eerror("pthread_create() error");

    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    return 0;
}
