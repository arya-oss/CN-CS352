#include <stdio.h>
#include <string.h>
int main() {
	char str[1024];
	memset(str, 65, 10);
	printf("%s", str);
	return 0;
}