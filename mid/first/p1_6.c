#include <stdio.h>
#include <poll.h>

int main() {
	char str[1024];
	while(1){
		read(0, str, 1024);
		printf("P6:%s\n", str);
		// sleep(1);
	}
	return 0;
}