#ifndef _P1_H
#define _P1_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>

#define shmkey1 0x123
#define shmkey2 0x124
#define semkey 0x134

int shmid1, shmid2, semid;

void create_semaphore(){
	semid = semget(semkey, 2, IPC_CREAT|0666);
	if(semid == -1){
		printf("Error in creating semaphore !\n");
		exit(2);
	}
}

void sem_init(){
	semctl(semid, 0, SETVAL, 0);
	semctl(semid, 1, SETVAL, 0);
}

void sem_wait(int semno){
	struct sembuf sb = {semno, -1, 0};
	semop(semid, &sb, 1);
}

void sem_post(int semno){
	struct sembuf sb = {semno, 1, 0};
	semop(semid, &sb, 1);
}

void create_shared_memory(){
	shmid1 = shmget(shmkey1, 4, IPC_CREAT|0666);
	if(shmid1 == -1){
		printf("Error in creating shared memory !\n");
		exit(1);
	}
	shmid2 = shmget(shmkey2, 4, IPC_CREAT|0666);
	if(shmid2 == -1){
		printf("Error in creating shared memory !\n");
		exit(1);
	}
}

int * attach_shared_memory1(){
	int * ch = (int *)shmat(shmid1, NULL, 0);
	if(ch == (int *)-1){
		printf("Error in attaching memory1\n");
		exit(1);
	}
	return ch;
}

int * attach_shared_memory2(){
	int * ch = (int *)shmat(shmid2, NULL, 0);
	if(ch == (int *)-1){
		printf("Error in attaching memory2\n");
		exit(1);
	}
	return ch;
}

#endif // _P1_H