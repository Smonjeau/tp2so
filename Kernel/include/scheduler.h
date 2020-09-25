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


typedef struct procQueue {
	PCB first;
	PCB last;
} procQueue;


/*  Debe ser llamada al crearse un nuevo proceso.
	En caso de exito retorna 0.
	En caso de error retorna -1 (por falta de memoria dinamica).
*/
int newProc(void * contextRSP);


#endif /* SCHEDULER_H_ */
