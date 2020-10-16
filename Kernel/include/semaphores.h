#ifndef SEMAPHORES_H
#define SEMAPHORES_H
int createSemaphore(int id, int initValue);

int waitSemaphore(int id);

int postSemaphore(int id);

int deleteSemaphore(int id);
#endif