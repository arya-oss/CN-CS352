#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }
#define sfifo "c2s_fifo.dat"

int main (int argc, char * argv[]){
    if(argc == 1)
        eerror("Usage: ./client <unique-name>");
    char buf[128], tmp[128];
    int fd = open(sfifo, O_WRONLY);
    write(fd, argv[1], strlen(argv[1]));
    close(fd);
    
    strcpy(tmp, argv[1]);
    strcat(tmp, "r.dat");
    mkfifo(tmp, 0666);

    strcpy(buf, argv[1]);
    strcat(buf, "w.dat");
    mkfifo(buf, 0666);
    
    int fd_r = open(tmp, O_RDONLY);
    if(fd_r == -1) eerror("fd_r error");
    printf("open %s\n", tmp);
    
    int fd_w = open(buf, O_WRONLY);
    if(fd_w == -1) eerror("fd_w error");
    printf("open %s\n", buf);
    
    int pid;
    pid = fork();
    if(pid >0) { // parent process
        close(fd_r);
        while(1){
            memset(tmp, 128, '\0');
            scanf("%s", tmp);
            write(fd_w, tmp, strlen(tmp)+1);
        }
    } else { // child process
        close(fd_w);
        while(1){
            memset(buf, 128, '\0');
            read(fd_r, buf, 128);
            printf("%s\n", buf);
        }
    }
    return 0;
}
