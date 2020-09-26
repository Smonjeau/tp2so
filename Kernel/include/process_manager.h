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



/* Updates de RSP of the running process and returns the RSP of the next process */

void *schedule(void *currProcRSP);

/* Calls scheduler and updates the RSP to the chosen process context */

void switchProcess();


/* Creates a context for a new process and returns its RSP */

void *createContext(int argc, char **argv, void *main);

/* Creates a new process and returns */

int newProcess(int argc, char **argv, void * main);


/* Kills a process and returns */

int killProcess(int pid);


#endif /* SCHEDULER_H_ */
