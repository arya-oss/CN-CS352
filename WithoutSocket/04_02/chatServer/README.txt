===========================================================
                DYNAMIC CHAT SERVER
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> create a chat server. when a new process
is created, it creates two fifo file whose prefix is cmd argument 1
for reading -> <arg>r.dat
for writing -> <arg>w.dat

Also it opens global fifo server and writes cmd argument 1 in
the fifo. Server opens both file in vice-versa way and stores
into two different array which is struct pollfd array and one
is integer array. pollfd array is used as arguments for poll 
syscall, pollfd array is for monitoring input data in any fifos
integer fd array is used for writing chat messages from one client
to all clients.

Compile:
gcc server.c -o server
gcc client.c -o client

RUN:( execute each command in a new terminal )
./server
./client p1
./client p2
./client p3
...

NOTE:
Dont't Quit clients by CTRL-C or any way, By doing this other client may
fall into infinite loop or server may quits.
Reason for this. I'm saving client writing and reading fifo in an array of fds.
when any clients exits, server doesn't know about this and checks for i/o which
may cause crash.
 
I think this program is not fully working.
if found any problem with it. Do debug by yourself.
And make a PULL REQUEST.

mail: arajmani@student.nitw.ac.in, rajmani1995@gmail.com
