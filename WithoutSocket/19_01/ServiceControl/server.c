#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/ipc.h>


#define eerror(msg) { printf("%s\n", msg); exit(1); }
#define sfifo "c2s_fifo.dat"
#define MAX 10
#define SHMKEY 0x123
#define SHMKEY1 0x124

int shmid,shmid1,fd; char buf[512], tmp[128];
char * sbuf;
int nClient=0;
char * args;

int main (){
    int i,j;
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
    for(i=0; i<10; i++){
        *(serv_id+i) = 1;
    }
    mkfifo(sfifo, 0666); 
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
        for(i=0; i<3; i++){
            if(*(serv_id+5+i) == 0){
                *(serv_id+5+i) = 1;
                sprintf(sbuf, "%s", buf);
                kill(*(serv_id+i), SIGUSR1);
                break;
            }
        }
        if(i == 3) {
            if(kill(atoi(args), SIGTERM) != -1)
                printf("No Service.. closing client.\n");
            else {
                printf("%d\n", atoi(args));
            }
        }
        nClient++;
        printf("Req success %d\n", nClient);
    }
    return 0;
}
