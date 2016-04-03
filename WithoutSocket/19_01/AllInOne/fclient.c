#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }   

int main (){
	char buf[32]; 
	int fd = open("c1.dat", O_WRONLY);
    if(fd == -1) eerror("open() error");
    while(1) {
    	memset(buf, 32, '\0');
    	scanf("%s", buf);
    	write(fd, buf, 32);
    }
    return 0;
}
