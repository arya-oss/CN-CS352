#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define BUFSIZE 128
#define eerror(msg) { printf("%s\n", msg); exit(1); }


volatile int nClient=0; char buffer[BUFSIZE]; 

int main(int argc, char const *argv[]) {
    struct sockaddr_in s_addr, c_addr;
    int sfd, _sfd, cli_len, i, n, port;
	
    int shmkey = shmget(0x1234, 4, 0666|IPC_CREAT);
	
    if(shmkey == -1 ) eerror("shmget() error");
	
    int * pid = (int*) shmat(shmkey, NULL, 0);
	
    if(pid == (int *)-1) 
        eerror("shmat() error");        
    
    if(argc == 1)
        eerror("Usage: ./service <port>")
    else
        port = atoi(argv[1]);

    memset((char *) &s_addr, 0, sizeof(s_addr));
    
    if ((sfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
        eerror("socket() error");
    
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons((u_short) port);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if( bind(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
        eerror("bind() error");

    if(listen(sfd, 5) < 0)
        eerror("listen() error");
    
    printf("Service Listening on port %d ...\n", port);
    int ppid = getppid();
    printf("%d\n", ppid);
    while(1) {
        cli_len = sizeof(c_addr);
        if((_sfd = accept(sfd, (struct sockaddr *) &c_addr, &cli_len)) < 0)
            eerror("accept() error");
        if(fork() == 0) {
            close(sfd);
            dup2(_sfd, 0);
            dup2(_sfd, 1);
            memset(buffer, '\0', BUFSIZE);
            read(0, buffer, BUFSIZE);
            if(strcmp(buffer, "exit") == 0)
                break;
            for (i = 0; i < strlen(buffer); ++i) {
                if(islower(buffer[i]))
                    buffer[i] = buffer[i]-32;
            }
            write(1, buffer, strlen(buffer));
        } else {
           	wait();
           	* pid = port;
           	if(kill(ppid, SIGUSR1) == -1)
                eerror("kill() error");
        }
        close(_sfd);
        nClient++;
        printf("Client %d connected. !!\n", nClient);
    }
    return 0;
}
