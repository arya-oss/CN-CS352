#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define aerror(msg) { printf(msg); exit(1); }
/**
 * Author: Rajmani Arya
 * Title: redirect.c
 * this four functions are provided to change input
 * or output redirection any time in your program with
 * any file.
 * It is very useful in IPC and Networking Programming.
 * NOTE: Here switching is done with directly filename
 * If anyone wants same thing with file descriptors.
 * 
 * void switchStdin(const int newfd)
 * {
 * 	   fflush(stdin);
 * 	   fgetpos(stdin, &pos_in);
 * 	   fd_in = dup(fileno(stdin));
 * 	   dup2(newfd, fileno(stdin));
 * }
 */
static int fd_in, fd_out;
static fpos_t pos_in, pos_out;

void switchStdin(const char *newStream)
{
  	fflush(stdin);
  	fgetpos(stdin, &pos_in);
  	fd_in = dup(fileno(stdin));
  	freopen(newStream, "r", stdin);
}

void revertStdin()
{
  	fflush(stdin);
  	dup2(fd_in, fileno(stdin));
  	close(fd_in);
  	clearerr(stdin);
  	fsetpos(stdin, &pos_in);
}

void switchStdout(const char *newStream)
{
  	fflush(stdout);
  	fgetpos(stdout, &pos_out);
  	fd_out = dup(fileno(stdout));
  	freopen(newStream, "w", stdout);
}

void revertStdout()
{
  	fflush(stdout);
  	dup2(fd_out, fileno(stdout));
  	close(fd_out);
  	clearerr(stdout);
  	fsetpos(stdout, &pos_out);
}

int main(int argc, char * argv[]) {
	/*switchStdin("in.txt");
	switchStdout("out.txt");
	char buf[128];
	scanf("%s", buf);
	printf("%s\n", buf);
	revertStdin();
	revertStdout();
	scanf("%s", buf);
	printf("%s\n", buf);
	*/char tmp[128];
    char buf[32] = "Hello";
    sprintf(tmp, "%d %s", 4, buf);
    printf("%s\n", tmp);
    return 0;
}
