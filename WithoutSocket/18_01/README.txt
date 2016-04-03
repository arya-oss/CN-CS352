========================================================
First Assignment (Basics of Shared Memory and Signals)
Author: Rajmani Arya, CSE 6th Sem
========================================================
1. Create two process p1 & p2 and both process opens two
shared memory x(int) and y(int). first p1 writes 0 to x
after that p2 reads x and writes y = x+1, now p1 reads y
and writes x = y+1 and process continues.

files_required -> p1_1.c, p1_2.c and p1.h
Compile:
	gcc p1_1.c -o p1
	gcc p1_2.c -o p2
RUN: (execute each command in a new terminal)
	./p1
	./p2

2. Problem statment is same as problem but instead of semaphores
 use SIGNAL ONLY. (SIGSTOP and SIGCONT is used for stoping and resuming
 processees.)
 
 First p1 -> opens shared memory x and writes own PID (process id) to
 and p2 reads x and save p1_pid and writes p2_process id to shared
 memory y and p1 reads it and save it as p2_pid.

files_required -> p2_1.c, p2_2.c and p2.h
Compile:
	gcc p2_1.c -o p1
	gcc p2_2.c -o p2
RUN: (execute each command in a new terminal)
	./p1
	./p2
	
3. Create Pipe features same as used in linux.
   e.g ./pipe p1 p2 p3 p4 .....
   output of p1 process goes input to p2, p2 o/p goes i/p to p3 and this
   continues until last process and last process writes output to stdout
   e.g
   p1.c
   #include <stdio.h>
   int main() {
   		char buf[128];
   		scanf("%s", buf);
   		printf("%s", buf);
   		return 0;
   }
compile:
	gcc pipe.c -o pipe
	gcc p1.c -o p1
	cp p1 p2
	cp p1 p3
RUN:
	./pipe p1 p2 p3
