#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <unistd.h>
#include <poll.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
/* contains all useful functions */
#include "utils.h"

int pusfd[3], usfd, rsfd;
int aval[3], pids[3];

int getPIDfromUsfd(int _usfd){
	struct ucred cred;
	int len = sizeof(cred);
	if(getsockopt(_usfd, SOL_SOCKET, SO_PEERCRED, &cred, &len) < 0) {
		perror("getsockopt() ");
	}
	return cred.pid;
}

int getPlatform(int pid) {
	int i;
	for (i = 0; i < 3; i++) {
		if(pid == pids[i])
			return i;
	}
	return -1;
}

void platformInit() {
	usfd = unixsock_bind("/tmp/sockpath");
	if(usfd < 0) {
		perror("unixsock_bind() ");
	}

	int i;
	for (i = 0; i < 3; i++) {
		pusfd[i] = unixsock_accept(usfd);
		if(pusfd[i] == -1) {
			perror("unixsock_accept() ");
		}
		pids[i] = getPIDfromUsfd(pusfd[i]);
	}
	close(usfd);
}

typedef void InfoHandler(int, siginfo_t *, void *);

void sig_handler(int sig, siginfo_t* info, void* vp) {
	int p = getPlatform(info->si_pid);
    printf("Train left from the platform %d\n", p+1);
	aval[p] = 0; // platform is available now
}

/* imported from http://stackoverflow.com/questions/15130660/c-get-process-id-of-signalling-process */
InfoHandler* SignalWithInfo(int signum, InfoHandler* handler) {
    struct sigaction action, old_action;
    memset((char *)&action, 0, sizeof(struct sigaction));
    action.sa_sigaction = handler;
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART|SA_SIGINFO; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        perror("Signal error");
    return (old_action.sa_sigaction);
}

int main() {
	/* Modified signal command for getting signal with signaling process process id
		Through this we can know which platform is free now */
	SignalWithInfo(SIGUSR1, sig_handler);
	/* making sockaddr_in for Annoucments */
	struct sockaddr_in addr;
	memset((char *)&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ports[] = { 8001, 8002, 8003 };
	struct pollfd ends[3]; int i, sfd, nsfd, j, ret, cli_len; char buf[128], tmp[128];
	struct sockaddr_in s_addr, c_addr;
	for (i = 0; i < 3; ++i) {
		sfd = tcpsock_bind("127.0.0.1", ports[i], 5);
		if(sfd < 0){
			perror("tcpsock_bind() ");
		}
	    printf("%d Ends listening ..\n", ports[i]);
	    ends[i].fd = sfd;
	    ends[i].events = POLLIN;
		aval[i] = 0; // All platforms is available
	}
	rsfd = socket(AF_INET, SOCK_RAW, 146); // initialize raw socket on protocol 146
	if(rsfd < 0) {
		perror("socket() ");
	}
	platformInit();
	printf("Station started listening ...\n");
	while(1) {
		ret = poll(ends, 3, 100);
		if(ret < 0) {
			sleep(0.5);
			continue;
		}
		for (i = 0; i < 3; ++i) {
			if(ends[i].revents & POLLIN) {
				printf("Req on %d\n", ports[i]);
				nsfd = tcpsock_accept(ends[i].fd);
				if(nsfd < 0) {
					perror("tcpsock_accept() ");
					continue;
				}
				printf("Train Accepted\n");

				sprintf(tmp, "Train coming from %d end", ports[i]);

				if(sendto(rsfd, tmp, strlen(tmp), 0, (struct sockaddr *)&addr, sizeof addr) < 0) {
            		perror("sendto() ");
        		}// Make Annoucment of incoming trains
				printf("Annoucment sent !\n");
				int flag=1;
				for(j=0; j<3; j++) {
					if(aval[j] == 0) {
						flag = 0;
						aval[j] = 1;
						sprintf(buf, "Platform %d", j+1);
						// Got a free platform
						send(nsfd, buf, strlen(buf), 0);
						if(send_fd(pusfd[j], nsfd) < 0) {
							perror("send_fd() ");
						}
						sleep(0.2);
						close(nsfd);
						break;
					}
				}
				if(flag==1) {
					// Platform not available
					printf("Platform not available. !\n");
					send(nsfd, "-1", 2, 0);
					close(nsfd);
				}
			}
		}
	}
	return 0;
}
