#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
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
	int rfd[3];
	
    fd = open("c5.dat", O_WRONLY);
    
    if(fd == -1) eerror("open() error");
    int pfd[2],i; char buf[1024];
    pipe(pfd);
    if(fork() == 0) {
        close(pfd[0]);
        for(i=0; i<1000; i++) {
            sprintf(buf, "child %d", i);
            write(pfd[1], buf, strlen(buf));
            sleep(2);
        }
    } else {
        close(pfd[1]);
        char tmp[32];
        if(fork() == 0) {
            scanf("%s", tmp);
            printf("stdin %s\n", tmp);
        } else {
            FILE * f = popen("./pclient", "r");
            rfd[0] = pfd[0];
            rfd[1] = open("c1.dat", O_RDWR);
            rfd[2] = fileno(f);
            int res, flag=0;
            fd_set rfds; struct timeval tv; tv.tv_sec=1; tv.tv_usec=0;
            while(1){
            	FD_ZERO(&rfds); memset(buf, 32, '\0');
                FD_SET(rfd[0], &rfds);
                res = select(rfd[0]+1, &rfds, NULL, NULL, &tv);
                if(res > 0 && FD_ISSET(rfd[0], &rfds)) {
                    read(rfd[0], buf, 32);
                    // printf("pipe\n");
                    printf("pipe %s\n", buf);
                }
                FD_ZERO(&rfds);
                FD_SET(rfd[1], &rfds);
                res = select(rfd[1]+1, &rfds, NULL, NULL, &tv);
                if(res > 0 && FD_ISSET(rfd[1], &rfds)) {
                    read(rfd[1], buf, 32);
                    // printf("fifo\n");
                    printf("fifo %s\n", buf);
                }
                if(flag) continue;
                FD_ZERO(&rfds);
                FD_SET(rfd[2], &rfds);
                res = select(rfd[2]+1, &rfds, NULL, NULL, &tv);
                if(res > 0 && FD_ISSET(rfd[2], &rfds)) {
                    read(rfd[2], buf, 32);
                    //printf("popen\n");
                    flag=1;
                    printf("%s\n", buf);
                }
            }
        }
    }
    return 0;
}
