#ifndef SCHEDULER_H_
#define SCHEDULER_H_

typedef enum ProcState{READY=0, RUN=1, BLOCKED=2, DEAD=3} ProcState;

typedef struct PCB {
    //int pid;
    ProcState procState;
    void * contextRSP;
    unsigned char remainingTicks;
    struct PCB * nextPCB;
} * PCB;


typedef struct ProcQueue {
	PCB first;
	PCB last;
} ProcQueue;


/* Returns the RSP of the next process */

void *schedule();

/* Creates a new process and returns */

int newProc(int argc, char **argv, void * main);

#endif /* SCHEDULER_H_ */
