typedef enum ProcState{READY=0, RUN=1, BLOCKED=2, DEAD=3} ProcState;

typedef struct PCB{
    int pid;
    ProcState procState;
    void *contextRSP;
} PCB;

PCB *pcbList;

void *schedule(void *currContextRSP){

    // Update the contextRSP of the running process and change it's procState to READY

    // Choose a new process and change it's procState to RUN

    // Return the contextRSP of the chosen process

}