#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }
#define sfifo "c2s_fifo.dat"
#define MAX 10

int fd; char buf[128], tmp[128];
int rfd[MAX], wfd[MAX], nClient=1;
void acceptReq(const char arg[]){
    printf("Got a req %s\n", arg);
    sprintf(tmp, "%sr.dat", arg);
    wfd[nClient] = open(tmp, O_WRONLY);
    if(wfd[nClient] == -1) eerror("WERROR!!!");
    
    sprintf(tmp, "%sw.dat", arg);
    rfd[nClient] = open(tmp, O_RDONLY);
    if(rfd[nClient] == -1) eerror("RERROR!!!");
    nClient++;
    printf("Req success %d\n", nClient);
}

int main (){
    mkfifo(sfifo, 0666); int i,j;
    fd = open(sfifo, O_RDWR);
    if(fd == -1) eerror("sfifo error");
    rfd[0] = fd;
    fd_set rfds;
    struct timeval tv; tv.tv_sec=2; tv.tv_usec=0;
    int max_fd, ret;
    while(1){
    	FD_ZERO(&rfds);
    	max_fd=0;
    	for(j=0; j<nClient; j++) {
    		if(rfd[j] > max_fd)
    			max_fd = rfd[j];
    		FD_SET(rfd[j], &rfds);
    	}
    	ret = select(max_fd+1, &rfds, NULL, NULL, &tv);
    	if(ret <=0 )
    		continue;
    	for(i=0; i<nClient; i++){
            if(FD_ISSET(rfd[i], &rfds)){
                memset(buf, 128, '\0');
                read(rfd[i], buf, 128);
                if (i==0){
                    acceptReq(buf);
                    //printf("%s\n", buf);
                } else {
                    sprintf(tmp, "P%d: %s", i, buf);
                    for(j=1; j<nClient; j++){
                        if(j != i)
                        write(wfd[j], tmp, 128);
                    }
                }
            }
        }
    }
    return 0;
}