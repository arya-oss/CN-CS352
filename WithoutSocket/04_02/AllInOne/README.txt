===========================================================
                All In One (Popen, fifo, pipe and signal)
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> create a server which is connected by each
process with each one of these methods(popen, fifo, pipe). and
gives output on stdout of server. if any process made a SIGUSR1
signal it redirects output to another process.

fifo -> c1.dat (communication b/w fifo process and server)
fifo -> c5.dat (communication b/w output process and server)

fclient.c -> fifo communicated process.
pclient.c -> popen communicated process.
oclient.c -> process which displays output of server after signal
Inside server a child process is created for pipe communication.

signaling process is not created.
It can be done directly from terminal.
open a new terminal
```bash
	ps -el|grep server # search process table server process and remember Parent Server process id
	# there may be 2 or more server process so choose only Parent Server
	kill -10 <process-id> # for redirecting o/p to o/p process
	kill -12 <process-id> # for reverting o/p of server
```

Compile:
./exec.sh

RUN:( execute each command in a new terminal )
./server
./fclient
./oclient
 kill -10 <process-id>
...

NOTE:
I think this program is not fully working.
if found any problem with it. Do debug by yourself and Make a PULL REQUEST

mail: arajmani@student.nitw.ac.in rajmani1995@gmail.com
