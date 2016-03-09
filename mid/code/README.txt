===========================================================
                All In One Sockets Chat
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> A server process S gets input data from process p1, p2, p3
and from stdin. P1 is connected through pipe and P2 is connected through FIFO.
Process P2 sends standard output to S, The Server also listens requests from tcp
clients on socket descriptor sfd and accepts any such request. If Server gets data
from sydin or any of three process, it sends same data to all socket clients. if S
gets a signal from P4 then it handovers (firrst come first serve) a socket client to
a newly created echo service, now this socket served by echo service only.After exiting of
echo service socket again acting like previously. (do without thread)

Compile:
./build

RUN:( execute each command in a new terminal )
./server 9500
./p2
./client 9500
./client 9500
...
NOTE:
if found any problem with it, do suggest !
mail: arajmani@student.nitw.ac.in rajmani1995@gmail.com
