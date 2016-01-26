#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }

static int fd_out;
static fpos_t pos_out;
int fd;

void switchStdout()
{
    fflush(stdout);
    fgetpos(stdout, &pos_out);
    fd_out = dup(fileno(stdout));
    dup2(fd, fileno(stdout));
}

void revertStdout()
{
    fflush(stdout);
    dup2(fd_out, fileno(stdout));
    close(fd_out);
    clearerr(stdout);
    fsetpos(stdout, &pos_out);
}

void handler1 (int signo) {
    printf("switching stdout\n");
    switchStdout();
    signal(SIGUSR1, handler1);
}

void handler2 (int signo) {
    printf("reverting stdout\n");
    revertStdout();
    signal(SIGUSR2, handler2);
}

int main (){
    signal(SIGUSR1, handler1);
    signal(SIGUSR2, handler2);
    
    mkfifo("c5.dat",0666);
    mkfifo("c1.dat", 0666);

    fd = open("c5.dat", O_WRONLY);
    if(fd == -1) eerror("open() error");
    int pfd[2],i; char buf[32];
    pipe(pfd);
    if(fork() == 0) {
        close(pfd[0]);
        for(i=0; i<1000; i++) {
            sprintf(buf, "child %d", i);
            write(pfd[1], buf, strlen(buf)+1);
            sleep(2);
        }
    } else {
        close(pfd[1]); struct pollfd pol[3];
        char tmp[32];
        if(fork() == 0) {
            scanf("%s", tmp);
            printf("stdin %s\n", tmp);
        } else {
            FILE * f = popen("./pclient", "w");
            pol[0].fd = pfd[0]; pol[0].events=POLLIN;
            pol[1].fd = open("c1.dat", O_RDONLY); pol[1].events=POLLIN;
            pol[2].fd = fileno(f); pol[2].events = POLLIN;
            while(1){
                int res = poll(pol, 3, 100);
                if(res <= 0){
                    continue;
                }
                if(pol[0].revents&POLLIN) {
                    read(pol[0].fd, buf, 32);
                    printf("pipe %s\n", buf);
                } else if(pol[1].revents & POLLIN){
                    read(pol[1].fd, buf, 32);
                    printf("fifo %s\n", buf);
                } else if(pol[2].revents & POLLIN) {
                    fgets(buf, 32, f);
                    printf("popen %s\n", buf);
                }
            }
        }
    }
    return 0;
}
