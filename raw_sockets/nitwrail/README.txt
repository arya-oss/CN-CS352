============================================================
        NITW Railway Junction Server

Author: Rajmani Arya
============================================================
See: problem.jpg for more description.

Problem Statment -> Create a railway station server, which listens
in 3 way (hyderabad, vijaywada and delhi end) and having 3 platforms.
At a time, one platform can serve only one train from any one of 3 ways.
First train ask station for available platform, if any platform not available
it reply "-1" and train has to try again, else platform available it pass socket fd
to platform and platforms recieves bogey sequence from train and display it.
train acknowledges platform and platforms acknowledges station that train leaving.
platforms also stream ads from cable tv and stream announcements from station.

In this problem statment any of the 5 IPC (pipe, fifo, shared mem, msg queue, semaphore) is not used.

Used APIs
BSD sockets
    TCP sockets
        Since Client is outside the system i.e on a network, so clients connects with station on any one
        of ends (H, V, D is listened by station)  with tcp sockets.
        port 8001 -> hyderabad end
        port 8002 -> Vijaywada end
        port 8003 -> Delhi end
    RAW sockets
        station want to broadcast announcements and cable tv also broadcast advertisments
        so with raw sockets they agrees on a protocol no. and recieve msgs.
        for announcements -> 146 protocol used.
        for advertisments -> 148 protocol used.
Pthread Library
   used only in platform code.
   used for listening announcements from station and advertisment from Cable TV.
Unix Domain sockets
    used to send trains socket file descriptor to platforms.

Key Points:
    for getting process id of platforms for station and vice-versa from unix socket fd
    struct ucred cred;

    int len = sizeof(cred);
	if(getsockopt(_usfd, SOL_SOCKET, SO_PEERCRED, &cred, &len) < 0) {
		perror("getsockopt() ");
	}
	return cred.pid;

    for getting process id of signaling process instead of signal Custom Signal is used.

    typedef void InfoHandler(int, siginfo_t *, void *);
    InfoHandler* SignalWithInfo(int signum, InfoHandler* handler) {
        struct sigaction action, old_action;
        memset((char *)&action, 0, sizeof(struct sigaction));
        action.sa_sigaction = handler;
        sigemptyset(&action.sa_mask); /* block sigs of type being handled */
        action.sa_flags = SA_RESTART|SA_SIGINFO; /* restart syscalls if possible */

        if (sigaction(signum, &action, &old_action) < 0)
            perror("Signal error");
        return (old_action.sa_sigaction);
    }

So, with these two things, any of the 5 IPC's is not needed.

COMPILE:
    ./build

RUN: (open each command in a new terminal)
    sudo ./station
    sudo ./platform 1
    sudo ./platform 2
    sudo ./platform 3
    sudo ./cable
    ./train 8001
    ./train 8002
    ./train 8003

NOTE:
    without initializing all platforms station can not accept trains.
    Use Terminator instead of gnome-terminal, terminator can be splited
    into many parts so no need of opening 8 windows terminals.
    to install terminator:
    sudo apt-get install terminator
    right click on terminator window and click either split horizontaly of verticaly.
