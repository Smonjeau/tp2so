#ifndef SEMAPHORES_H
#define SEMAPHORES_H

int openSemaphore(int id, int initValue);

int waitSemaphore(int id);

int postSemaphore(int id);

int closeSemaphore(int id);

void semStatus(void * buffer, int * qty);

void getBlockedProc(char * buffer, int id);

#endif