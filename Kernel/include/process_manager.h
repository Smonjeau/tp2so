#ifndef PROCESS_MANAGER_H_
#define PROCESS_MANAGER_H_
#include <pipe_manager.h>
#define NULL (void*)0
#define MAX_PIPES 10
typedef enum ProcState{READY=0, RUN=1, BLOCKED=2, DEAD=3} ProcState;
typedef struct PCB {
    int pid;
    ProcState procState;
    void * contextRSP;
    void * baseRSP;
    char * name;
    unsigned char remainingTicks;
    pipe pipes [MAX_PIPES];
    struct PCB * nextPCB;

} * PCB;

// x/8xb runningProc



typedef struct ProcQueue {
	PCB first;
	PCB last;
} ProcQueue;



// Updates de RSP of the running process and returns the RSP of the next process

void * schedule(void *currProcRSP);

// Calls schedule and updates the RSP to the chosen process context

void switchProcessContext();



// Creates a PCB for a new process and returns its PID

int createProcessPCB(void * contextRSP, void * baseRSP, char * name);

// Creates a context for a new process, calls createProcessPCB and returns its PID

int createProcessContext(int argc, char **argv, void *main, char * name);

// Creates a context for a new process from Kernel, calls createProcessPCB and returns its PID
//It avoids setting interrupts

int createProcessContextFromKernel(int argc, char **argv, void *main);



/* Kills a process and returns */

void killProcess(int pid);


/* Switch process state READY/BLOCKED.*/

void blockProcess(int pid);


/* Change process priority (100 - 139) */

void niceProcess(int pid, int priority);


/* PS devuelve arreglo de structs PCB existentes y numero de procesos */
void ps(char * buffer/*, int * procCant*/);


/* Returns the PID of the running process, or -1 if none */

int getPID();


/* Busca dos fds libres en el pcb del proceso y los asigna*/

int assign_pipe_to_pcb(int * fds, pipe pipe);

void close_fd(int fd);


#endif