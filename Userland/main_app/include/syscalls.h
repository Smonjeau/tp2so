#include <stdint.h>

// ------------------------------------ I/O BUFFERS ------------------------------------

int read(int fd, char * buffer, int n);

int write(int fd, char * buffer, int n);


// ------------------------------------- MISCELLANEOUS -------------------------------------

int draw(int x, int y, int rgb);

typedef struct ScreenRes{
    int height, width;
} ScreenRes;


int getRes(ScreenRes * res);

typedef struct Time{
    int hours, minutes, seconds;
} Time;

int getTime(Time * time);


// ------------------------------------- CPU INFO -------------------------------------

typedef struct CPUInfo{
    char *brandName, *brandDesc;
} CPUInfo;

int cpuInfo(CPUInfo * info);

int cpuTemp();

typedef struct RegBkp{
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
} RegBkp;

int getRegBkp(RegBkp * dest);


// ------------------------------------ MEMORY MANAGEMENT ------------------------------------

int memDump(void *src, void *dest);

void memStatus(int * total_mem, int * available_mem , int * occ_mem);

void * malloc (int size);

void free(void * ptr);


// ------------------------------------ PROCESS MANAGEMENT ------------------------------------

int startProcess(void *main, int argc, char **argv, char * name);

int getPid ();

void ps (char * buffer/*, int * procCant*/);

int block (int pid);

int nice(int pid, int newPrio);

int kill (int pid);

typedef enum ProcState{READY=0, RUN=1, BLOCKED=2, DEAD=3} ProcState;


// ------------------------------------ SYNCHRONIZATION ------------------------------------

int createSem(int id, int initValue);

int waitSem(int id);

int postSem(int id);

int deleteSem(int id);

int pipe(int * fds);

int forcePipe(int fd);

void close(int fd);

int pipeRead (int fd, char * buffer, int bytes);