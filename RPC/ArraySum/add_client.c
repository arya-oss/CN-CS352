#include "add.h"

void addition_1(char *host)
{
	CLIENT *clnt;
	int  *result_1;
	numbers  add_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, ADDITION, ADDITION_1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	printf("Enter Array Size: ");
	scanf("%d", &add_1_arg.size);
	printf("Enter Numbers: ");
	int i;
	for(i=0; i<add_1_arg.size; i++){
		scanf("%d", &add_1_arg.nums[i]);
	}
	result_1 = add_1(&add_1_arg, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	printf("Sum: %d\n", *result_1);
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	addition_1 (host);
	exit (0);
}
