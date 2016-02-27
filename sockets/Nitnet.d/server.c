#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define size 128
#define MAX 3
#define SHMKEY 137146
#define eerror(msg) { printf("%s\n", msg); exit(1); }

typedef struct service
{
    int port;
    char * s_name;
    int id;
}service_t;

service_t serv[] = { {9501, "s1", 1}, {9502, "s2", 2}, {9503, "s3", 3} };

int get_serv_id(char * serv_name, int * id) {
    int i;
    for (i = 0; i < MAX; ++i){
        if(strcmp(serv_name, serv[i].s_name) == 0){
            * id = i;
            return serv[i].port;
        }
    }
    return i;
}

int max_client[MAX] = {2, 1, 2};
int clients[MAX] = {0};
int started[MAX] = {0};
int * pid;

void handler(int sig) {
	int i;
    printf("signal recieved %d\n", *pid);
	for (i=0; i<MAX; i++) {
		if (serv[i].port == *pid) {
			clients[i]--;
            break;
		}
	}
	printf("client disconnected %d !!\n", *pid);
}

int main(int argc, char const *argv[]) {
    int shmkey = shmget(0x1234, 4, 0666|IPC_CREAT);
	
    if(shmkey == -1 ) eerror("shmget() error");
	pid = (int *)shmat(shmkey, NULL, 0);
	if(pid == (int *)-1) eerror("shmat() error"); 
	
    struct sockaddr_in s_addr, c_addr;
    int sfd, _sfd, cli_len, n, i, port; char buf[size];
    if(argc == 1){
        port = 9500;
    } else {
        port = atoi(argv[1]);
        if(port < 1024 || port > 16383)
            eerror("Usage: ./server [portno]");
    }
    signal(SIGUSR1, handler);
    memset((char *) &s_addr, 0, sizeof(s_addr));
    if ((sfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
        eerror("socket() error");

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons((u_short) port);
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr)) < 0)
        eerror("bind() error");

    if(listen(sfd, 5) < 0)
        eerror("listen() error");
    printf("Super Server Listening on port %d ...\n", port);
    fd_set rfds;

    // while(1) {
    start:
        FD_ZERO(&rfds);
        FD_SET(sfd, &rfds);
        if (select(sfd+1, &rfds, NULL, NULL, NULL) > 0) {
            if(!FD_ISSET(sfd, &rfds))
                goto start;
                // continue;
            // printf("Ok\n");
            cli_len = sizeof(c_addr);
            if((_sfd=accept(sfd, (struct sockaddr *)&c_addr, &cli_len)) < 0){
                eerror("accept() error");
            }
            memset(buf, 0, sizeof(buf));
            read(_sfd, buf, size);
            int serv_id=MAX;
            // printf("%s %d\n", buf, strlen(buf));
            port = get_serv_id(buf, & serv_id);

            if(serv_id >= MAX || clients[serv_id] == max_client[serv_id]) {
                write(_sfd, "exit", 4);
                // continue;
                goto start;
            } else {
                sprintf(buf, "%d", port);
                write(_sfd, buf, strlen(buf));
                
                if(started[serv_id] == 0) {
                    started[serv_id] = 1;
                    if(fork() == 0) {
                        execl(serv[serv_id].s_name, serv[serv_id].s_name, buf, NULL);
                        eerror("execl() error");
                    } else
                        clients[serv_id]++;
                }
                
            }
            close(_sfd);
        }
        goto start;
    // }

    return 0;
}
