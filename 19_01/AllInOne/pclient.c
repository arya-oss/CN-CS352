#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }   

int main (){
	int i;
	for (i = 0; i<1000; ++i) {
		printf("message %d\n", i);
		sleep(2);
	}
    return 0;
}
