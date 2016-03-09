#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>

int main() {
	FILE * f; struct pollfd pfds[5]; int i, flag; char str[1024];
	f = popen("./p1_2", "r");
	pfds[0].fd = fileno(f);
	f = popen("./p1_3", "r");
	pfds[1].fd = fileno(f);
	f = popen("./p1_4", "r");
	pfds[2].fd = fileno(f);
	f = popen("./p1_5", "r");
	pfds[3].fd = fileno(f);
	f = popen("./p1_6", "w");

	for (i = 0; i < 4; ++i) {
		pfds[i].events = POLLIN;
	}
	printf("Opened all process\n");
	while(1) {
		int res = poll(pfds, 5, 100);
		if (res < 0){
			sleep(1);
			continue;
		}
		flag = 1;
		for (i = 0; i < 4; ++i){
			if (pfds[i].revents & POLLIN) {
				// printf("io ready %d\n", i+1);
				flag = 0;
				memset(str, 0, 1024);
				read(pfds[i].fd, str, 1024);
				dup2(fileno(f), 1);
				printf("%s\n", str);
				// fprintf(f, "%s", str);
				// dup2(fileno(f),pfds[i].fd);
				sleep(1);
				pfds[i].fd = -1;
			}
		}
		// if(flag) {
		//  	dup2(1, fileno(f));
		// }
	}
	return 0;
}