#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define eerror(msg) { printf("%s\n", msg); exit(1); }

int main() {
	char buffer[128]; int n,i;
	n = read(0, buffer, 128);
	for(i=0; i<n; i++)
        if(islower(buffer[i]))
            buffer[i] = buffer[i]-32;
    if(write(0, buffer, n) < 0 )
        eerror("write() error");
	return 0;
}
