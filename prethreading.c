#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void * runner1(void * args) {
	int * a = (int *) args;
	int i, sum=0;
	for (i = 0; i < *a; i+=2){
		sum += i;
	}
	printf("Even Sum: %d\n", sum);
}

void * runner2(void * args) {
	int * a = (int *) args;
	int i, sum=0;
	for (i = 1; i < *a; i+=2){
		sum += i;
	}
	printf("Odd Sum: %d\n", sum);
}

void * runner3(void * args) {
	int * a = (int *) args;
	int i, sum=0;
	for (i = 0; i < *a; i++){
		sum += i;
	}
	printf("Total Sum: %d\n", sum);
}

pthread_mutex_t lock;
volatile int head=-1, size=0;
#define max 10000

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
		if(head == size-1) {
			break;
		}
		func = job[++head].function;
		args1 = job[head].args;
		pthread_mutex_unlock(&lock);
		func(args1);
		sleep(1);
	}

}

int main(int argc, char const *argv[]) {
	pthread_t pd; int id[max];
	pthread_mutex_init(&lock, NULL);
	pthread_create(&pd, NULL, &runner, NULL);
	int i=0;
	for (int i = 1; i <= 10; ++i) {
		id[i] = i+1;
		job[size].function = &runner1;
		job[size++].args = &id[i];
		job[size].function = &runner2;
		job[size++].args = &id[i];	
		job[size].function = &runner3;
		job[size++].args = &id[i];		
		sleep(0.5);
	}
	
	pthread_join(pd, NULL);
	return 0;
}
