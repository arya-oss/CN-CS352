===========================================================
                NITW Railway Server
 Author: Rajmani Arya
 CSE 3rd Year, 6th Semester, NITW
===========================================================
Problem Statment -> Create a railway station server, which listens
in 3 way (hyderabad, vijaywada and delhi end) and having 3 platforms.
At a time one platform can serve only one train from any one of 3 ways.
First train ask station for available platform, if any platform not available
it reply "-1" and train has to try again, else platform available it reply
platform number (port no) of platform and signals to platform, where platforms
connects with train and recieves bogey sequence from train and display it. 
train acknowledge station that I'm done and leaves.

port 8001 -> for hyderabad end
port 8002 -> for vijaywada end
port 8003 -> for delhi end
port 8004 -> for platform 1
port 8005 -> for platform 2
port 8006 -> for platform 3


Compile:
./build

RUN:( execute each command in a new terminal )
./station
./train 8001 s1s2s3s4b1b2b3
...
./train <any one of 3 ways> <display bogey seq>
NOTE:
if found any problem with it, do suggest !
mail: arajmani@student.nitw.ac.in rajmani1995@gmail.com
