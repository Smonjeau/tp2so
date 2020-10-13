#include <stdint.h>

// ------------------------------------ MEMORY MANAGEMENT ------------------------------------

void memStatus(int * total_mem, int * available_mem , int * occ_mem);

void * malloc (int size);

void free(void * ptr);

int memDump(void *src, void *dest);


// ------------------------------------ PROCESS MANAGEMENT ------------------------------------

int startProcess(void *main, int argc, char **argv, char * name);

int kill (int pid);

int block (int pid);

int getPid ();

void ps (void * buffer, int * procCant);

typedef enum ProcState{READY=0, RUN=1, BLOCKED=2, DEAD=3} ProcState;

typedef struct PCB {
    int pid;
    ProcState procState;
    void * contextRSP;
    void * baseRSP;
    char * name;
    unsigned char remainingTicks;
    struct PCB * nextPCB;
} PCB;


// ------------------------------------- KEYBOARD -------------------------------------

int read();


// ------------------------------------- SCREEN -------------------------------------

int draw(int x, int y, int rgb);

typedef struct ScreenRes{
    int height, width;
} ScreenRes;


int getRes(ScreenRes * res);


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


// ------------------------------------- OTHERS -------------------------------------

typedef struct Time{
    int hours, minutes, seconds;
} Time;

int getTime(Time * time);