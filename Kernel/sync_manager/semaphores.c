#include <mem_manager.h>
#include <process_manager.h>
#include <lib.h>

#define MAX_BLOCKED_PIDS 20

typedef struct Semaphore{
    int id;
    int value;
    int blockedPIDs[MAX_BLOCKED_PIDS];
    int blockedPIDsSize;
    struct Semaphore * next;
} Semaphore;

Semaphore * semaphores = NULL;


int createSemaphore(int id, int initValue){

    Semaphore *newSem = malloc(sizeof(Semaphore));
    if(newSem == NULL)
        return -1;

    newSem->id = id;
    newSem->value = initValue;
    newSem->blockedPIDsSize = 0;
    newSem->next = NULL;

    Semaphore *lastSem = semaphores;
    if(lastSem == NULL){
        semaphores = lastSem = newSem;
    }else{
        while(lastSem->next != NULL)
            lastSem = lastSem->next;
        lastSem->next = newSem;
    }

    return 0;

}


Semaphore * findSemaphore(int id, int prev){
    
    Semaphore *semaphore = semaphores;
    if(!prev)
        while(semaphore!=NULL && semaphore->id != id)
            semaphore = semaphore->next;
    else
        while(semaphore!=NULL && semaphore->next!=NULL && semaphore->next->id != id)
            semaphore = semaphore->next;

    return semaphore;

}


int waitSemaphore(int id){

    Semaphore *semaphore = findSemaphore(id, 0);
    if(semaphore == NULL)
        return -1;

    if(semaphore->value > 0){
        semaphore->value -= 1;
        return 0;
    }

    int currPid = getPID();
    blockProcess(currPid);
    semaphore->blockedPIDs[semaphore->blockedPIDsSize++] = currPid;

    return 0;

}


int postSemaphore(int id){

    Semaphore *semaphore = findSemaphore(id, 0);
    if(semaphore == NULL)
        return -1;

    if(semaphore->value == 0){
        for(int i=0; i<semaphore->blockedPIDsSize; i++)
            blockProcess(semaphore->blockedPIDs[i]);
        semaphore->blockedPIDsSize = 0;
    }

    semaphore->value += 1;

    return 0;

}


int deleteSemaphore(int id){

    Semaphore *prevSemaphore = findSemaphore(id, 1);
    if(prevSemaphore == NULL)
        return -1;

    Semaphore *semaphore = prevSemaphore->next;
    if(semaphore->blockedPIDsSize > 0)
        return -1;

    prevSemaphore->next = semaphore->next;
    free(semaphore);

    return 0;

}