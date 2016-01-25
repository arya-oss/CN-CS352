===============================================================
                    Service Controller Server
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===============================================================

Problem Statment -> Create a Service that control services.

when a new client is created, it creates two fifo file whose name
are <arg-prefix>r.dat and <arg-prefix>w.dat and prfix is written
to global fifo of server.

In this problem Max Service instances is 5. Server choose a service
instance which is available and writes client fifo prefix in shared
memory and signals service which is free. service reads that shared
memory and open both fifo for clients. now client is ready for service
it can request service like ls, ps etc. To stop client writes 'exit'.
now service becomes free and ready to serve other client.

Compile:
gcc server.c -o server
gcc client.c -o client
gcc service.c -o service

RUN:( execute each command in a new terminal )
./server
./service 1
./service 2

./client c1
./client c2
...

For Fast compiling and removing fifo(.dat) files
execute the shell script given exec.sh
$ ./exec.sh or sh exec.sh

NOTE:
service command argument should not repeat and always
starts from 1
example:

For S1 -> ., -a, -l, /home, /, etc
For S2 -> -1, -3 etc
For S3 -> filename

before running the program again Please REMOVE fifo files.
cmd -> rm *.dat

If found any problem with it. do debug by yourself.

mail: arajmani@student.nitw.ac.in rajmani1995@gmail.com
