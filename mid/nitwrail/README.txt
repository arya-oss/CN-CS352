===========================================================
                NITW Railway Server
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> Create a railway station server, which listens
in 3 way (hyderabad, vijaywada and delhi end) and having 3 platforms.
At a time one platform can serve only one train from any one of 3 ways.
First train ask station for available platform, if any platform not available
it reply "exit" and train has to try again, else platform available it reply
platform number of platform and writes train fifo_name into shared memory and
signals to platform, where platforms recieves bogey sequence from train and display
it. train acknowledge station that I'm done and leaves.

h_fifo -> for hyderabad end
v_fifo -> for vijaywada end
d_fifo -> for delhi end
a_fifo -> for acknowledgement

Compile:
./build

RUN:( execute each command in a new terminal )
./station
./train h s1s2s3s4b1b2b3 t1
...
./train <any one of 3 ways> <display bogey seq> <train_id>
NOTE:
if found any problem with it, do suggest !
mail: arajmani@student.nitw.ac.in rajmani1995@gmail.com
