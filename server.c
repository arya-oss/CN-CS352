#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }
#define sfifo "c2s_fifo.dat"
#define MAX 10

int fd; char buf[128], tmp[128];
struct pollfd plfd[MAX]; int nClient=1;
int wfd[MAX];
void acceptReq(const char arg[]){
    printf("Got a req %s\n", arg);
    strcpy(tmp, arg);
    strcat(tmp, "r.dat");
    mkfifo(tmp, 0666);
    wfd[nClient] = open(tmp, O_WRONLY);
    if(wfd[nClient] == -1) eerror("ERROR!!!");
    printf("created and opened %s\n", tmp);
    strcpy(tmp, arg);
    strcat(tmp, "w.dat");
    mkfifo(tmp, 0666);
    plfd[nClient].fd = open(tmp, O_RDONLY);
    plfd[nClient].events = POLLIN;
    printf("created and opened %s\n", tmp);
    nClient++;
    printf("Req success %d\n", nClient);
}

int main (){
    mkfifo(sfifo, 0666); int i,j;
    fd = open(sfifo, O_RDONLY);
    if(fd == -1) eerror("sfifo error");
    plfd[0].fd = fd;
    plfd[0].events = POLLIN;
    
    while(1){
        int res=poll(plfd, nClient, 100);
        if(res > 0){
            for(i=0; i<nClient; i++){
                if(plfd[i].revents & POLLIN){
                    memset(buf, 128, '\0');
                    read(plfd[i].fd, buf, 128);
                    if (i==0){
                        acceptReq(buf);
                    } else {
                        printf("Send %s to all\n", buf);
                        for(j=1; j<nClient; j++){
                            if(j != i)
                            write(wfd[j], buf, strlen(buf)+1);
                        }
                    }
                    printf("Yahooo\n");
                }
            }
        } else{
            printf("poll not worked\n");
        }
    }
    return 0;
}
