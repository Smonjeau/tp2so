#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#define MAX_BLOCKED_PIDS 20

typedef struct Semaphore{
    int id;
    int value;
    int blockedPIDs[MAX_BLOCKED_PIDS];
    int blockedPIDsSize;
    int lock;
    int listeners;
    struct Semaphore * next;
} Semaphore;

int openSemaphore(int id, int initValue);

int waitSemaphore(int id);

int postSemaphore(int id);

int closeSemaphore(int id);

void semStatus(void * buffer, int * qty);

void getBlockedProc(char * buffer, int id);

#endif