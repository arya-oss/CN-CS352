#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }
#define sfifo "c2s_fifo.dat"

int fd_w, fd_r; char buf[128], tmp[128];

void writer(void * args) {
	while(1){
        memset(tmp, 128, '\0');
        fgets(tmp, 128, stdin);
        write(fd_w, tmp, 128);
    }
}

void reader(void * args) {
	while(1){
        memset(buf, 128, '\0');
        read(fd_r, buf, 128);
        printf("%s", buf);
    }
}

int main (int argc, char * argv[]){
    if(argc == 1)
        eerror("Usage: ./client <fifo-prefix>");
    pthread_t pid1, pid2;

    sprintf(tmp, "%sr.dat", argv[1]);
    mkfifo(tmp, 0666);

    sprintf(buf, "%sw.dat", argv[1]);
    mkfifo(buf, 0666);
    
    int fd = open(sfifo, O_WRONLY);
    write(fd, argv[1], strlen(argv[1]));  // Do request to server for connection
    close(fd);
    
    fd_r = open(tmp, O_RDONLY);
    if(fd_r == -1) eerror("fd_r error");
    
    fd_w = open(buf, O_WRONLY);
    if(fd_w == -1) eerror("fd_w error");

    printf("You are connected !\n");

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
