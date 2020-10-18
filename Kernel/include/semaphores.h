#ifndef SEMAPHORES_H
#define SEMAPHORES_H
int createSemaphore(int id, int initValue);

int waitSemaphore(int id);

int postSemaphore(int id);

int deleteSemaphore(int id);

void semStatus(void * buffer, int * qty);

void getBlockedProc(char * buffer, int id);
#endif