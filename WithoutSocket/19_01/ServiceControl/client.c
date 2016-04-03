#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }
#define sfifo "c2s_fifo.dat"

int fd_r, fd_w; char buf[512], tmp[128];

void writer(void * args) {
	while(1){
       	memset(tmp, 128, '\0');
        scanf("%s", tmp);
        write(fd_w, tmp, strlen(tmp)+1);
    }
}

void reader(void * args) {
	while(1){
        memset(buf, 512, '\0');
        if(read(fd_r, buf, 512) != 0)
            printf("%s\n", buf);
        sleep(0.1);
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
    sprintf(tmp, "%s %d", argv[1], getpid());
    write(fd, tmp, strlen(tmp));
    close(fd);
    
    sprintf(tmp, "%sr.dat", argv[1]);
    fd_r = open(tmp, O_RDONLY);
    if(fd_r == -1) eerror("fd_r error");
    //printf("open %s\n", tmp);
    
    fd_w = open(buf, O_WRONLY);
    if(fd_w == -1) eerror("fd_w error");
    //printf("open %s\n", buf);
    
    printf("ready to go\n");
    
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
