#ifndef PROCESS_MANAGER_H_
#define PROCESS_MANAGER_H_

#include <pipe_manager.h>

#define NULL (void*)0
#define MAX_PIPES 10

typedef enum ProcState{READY=0, RUN=1, BLOCKED=2, DEAD=3, RECENTLY_BLOCKED=4} ProcState;

typedef struct PCB {
    int pid;
    ProcState procState;
    void * contextRSP;
    void * segmentAddress;
    char * name;
    unsigned char remainingTicks;
    pipe pipes [MAX_PIPES];
    struct PCB * parent;
    struct PCB * nextPCB;

} * PCB;

typedef struct ProcQueue {
	PCB first;
	PCB last;
} ProcQueue;


// Updates de RSP of the running process and returns the RSP of the next process

void * schedule(void *currProcRSP);

// Calls schedule and updates the RSP to the chosen process context

void switchProcessContext();


// Creates a PCB for a new process and returns its PID

int createProcessPCB(void * contextRSP, void * baseRSP, char * name, int bg);

// Creates a context for a new process, calls createProcessPCB and returns its PID

int createProcessContext(int argc, char **argv, void *main, char * name, int bg);


/*Revis asi el proceso running tiene el fd indicado libre*/
int is_fd_free(int fd);


/* Kills a process and returns */

void killProcess(int pid);


/* Switch process state READY/BLOCKED.*/

void blockProcess(int pid,int tick);


/* Change process priority (100 - 139) */

void niceProcess(int pid, int priority);


/* PS devuelve arreglo de structs PCB existentes y numero de procesos */
void ps(char * buffer);


/* Returns the PID of the running process, or -1 if none */

int getPID();


/* Busca dos fds libres en el pcb del proceso y los asigna*/
int assign_pipe_to_pcb(int * fds, pipe pipe);


/* Asigna el pipe recibido al fd especificado del proceso running */
int assign_pipe_to_pcb_forced(int fd, pipe new);

/* Cierra cierto fd del proceso running */
void close_fd(int fd);

/*Cierra cierto fd de cierto proceso*/
void close_fd_proc(PCB pcb, int fd);

/* Mueve el fd recibido al fd mas chico libre */
int dup_fd(int fd);

/* Busca el pipe al cual esta coenctado el fd indicado */

pipe findPipe(int fd);

/*Recorre los procesos y retorna data de los pipes*/
void get_pipes_data(void * buffer, int * qty);


/* Copia el nombre del proceso en el buffer*/
void getProcName(int pid, char * buffer);


/* Returns whether the running process has the foreground */
int hasForeground();

#endif