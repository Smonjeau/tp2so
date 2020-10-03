#ifndef PROCESS_MANAGER_H_
#define PROCESS_MANAGER_H_

#define NULL (void*)0
typedef enum ProcState{READY=0, RUN=1, BLOCKED=2, DEAD=3} ProcState;

typedef struct PCB {
    int pid;
    ProcState procState;
    void * contextRSP;
    void * baseRSP;
    char * name;
    unsigned char remainingTicks;
    struct PCB * nextPCB;
} * PCB;




typedef struct ProcQueue {
	PCB first;
	PCB last;
} ProcQueue;



// Updates de RSP of the running process and returns the RSP of the next process

void *schedule(void *currProcRSP);

// Calls schedule and updates the RSP to the chosen process context

void switchProcessContext();



// Creates a PCB for a new process and returns its PID

int createProcessPCB(void * contextRSP, void * baseRSP, char * name);

// Creates a context for a new process, calls createProcessPCB and returns its PID

int createProcessContext(int argc, char **argv, void *main, char * name);

// Creates a context for a new process from Kernel, calls createProcessPCB and returns its PID
//It avoids setting interrupts

int createProcessContextFromKernel(int argc, char **argv, void *main);

//int getNumProc();


/* Kills a process and returns */

void killProcess(int pid);


/* PS devuelve arreglo de structs PCB existentes y numero de procesos */
void ps(void * buffer, int * procCant);


/* Devuelve el RSP "mas chico" */
void * getLastContext();


#endif 
