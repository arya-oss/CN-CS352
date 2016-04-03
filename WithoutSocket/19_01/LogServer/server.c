#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }
#define sfifo "c2s_fifo.dat"
#define MAX 10

int fd; char buf[512], tmp[128];
int nClient=0;
char * args, service[128];

int main (){
    mkfifo(sfifo, 0666); int i,j;
    fd = open(sfifo, O_RDONLY);
    if(fd == -1) eerror("sfifo error");
    
    while(1){
        memset(buf, 128, '\0');
        if(read(fd, buf, 128) == 0){
            sleep(0.5);
            continue;
        }
        
        printf("Got a req %s\n", buf);
        
        strtok_r(buf, " ", &args);
        strcpy(service, "./");
        strcat(service, buf);
        strcat(service, " ");
        strcat(service, args);
        
        if(fork() > 0){
            printf("%s %s %s\n", buf, args, service);
            FILE * f = popen(service, "w");
        }
        nClient++;
        printf("Req success %d\n", nClient);
    }
    return 0;
}
