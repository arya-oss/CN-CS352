#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void * runner1(void * args) {
	int * a = (int *) args;
	printf("runner %d\n", *a);
}

void * runner3(void * args) {
	int * a = (int *) args;
	printf("runner %d\n", *a);
}

void * runner2(void * args) {
	int * a = (int *) args;
	printf("runner %d\n", *a);
}

pthread_mutex_t lock;
volatile int head=-1, size=0;
#define max 3

struct job_t {
	void*(*function)(void *);
	void * args;
} job[max];

void * runner (void * args) {
	void*(*func)(void*);
	void * args1;
	while(1) {
		pthread_mutex_lock(&lock);
		if(size == 0) {
			printf("waiting\n");
			pthread_mutex_unlock(&lock);
			sleep(1);
			continue;
		}
		if(head == max-1) {
			break;
		}
		func = job[++head].function;
		args1 = job[head].args;
		pthread_mutex_unlock(&lock);
		printf("ok\n");
		func(args1);
	}

}

int main(int argc, char const *argv[]) {
	pthread_t pd; int id[max] = {1,2,3};
	pthread_mutex_init(&lock, NULL);
	pthread_create(&pd, NULL, &runner, NULL);
	job[0].function = & runner1;
	job[0].args = &id[0];
	size = 1;
	job[1].function = & runner2;
	job[1].args = &id[1];
	size = 2;
	job[2].function = & runner3;
	job[2].args = &id[2];
	size = 3;
	pthread_join(pd, NULL);
	return 0;
}