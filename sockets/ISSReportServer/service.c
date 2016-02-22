#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main() {
	char buf[128]; int i;
	//while(1) {
		read(0, buf, 128);
		for (i = 0; i < strlen(buf); ++i) {
			if (islower(buf[i]))
				buf[i] = buf[i]-32;
		}
		write(1, buf, strlen(buf));
		//memset(buf, 0, 128);
	//}
	sleep(1);
	return 0;
}
