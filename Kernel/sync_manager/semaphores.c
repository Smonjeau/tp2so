#include <mem_manager.h>
#include <process_manager.h>
#include <interrupts.h>
#include <lib.h>

#define MAX_BLOCKED_PIDS 20
typedef struct Semaphore{
    int id;
    int value;
    int blockedPIDs[MAX_BLOCKED_PIDS];
    int blockedPIDsSize;
    int lock;
    struct Semaphore * next;
} Semaphore;

Semaphore * semaphores = NULL;


void semStatus(void * buffer, int * sem_count){
    //Recorremos todos los semáforos y pasamamos el struct
    Semaphore * semaphore = semaphores;

    int count=0;
    int size= sizeof(Semaphore);
    while (semaphore!=NULL) {
        memcpy(buffer + size * count++, semaphore, size);
        semaphore = semaphore->next;
    }
    *sem_count=count;


}


int createSemaphore(int id, int initValue){

    Semaphore *newSem = malloc(sizeof(Semaphore));
    if(newSem == NULL)
        return -1;

    newSem->id = id;
    newSem->value = initValue;
    newSem->blockedPIDsSize = 0;
    newSem->lock = 0;
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


int waitSemaphore(int id) {

    Semaphore *semaphore = findSemaphore(id, 0);
    if(semaphore == NULL)
        return -1;
    
    acquire(&(semaphore->lock));

    if(semaphore->value > 0){
        semaphore->value -= 1;
    }else{
        release(&(semaphore->lock));

        int currPid = getPID();

        semaphore->blockedPIDs[semaphore->blockedPIDsSize++] = currPid;
        blockProcess(currPid,1);

        acquire(&(semaphore->lock));

        semaphore->value -= 1;

     //   _timer_tick();
    }

    release(&(semaphore->lock));

    return 0;

}


int postSemaphore(int id){

    Semaphore * semaphore = findSemaphore(id, 0);
    if(semaphore == NULL)
        return -1;

    acquire(&(semaphore->lock));

    semaphore->value += 1;
    int aux = semaphore->blockedPIDsSize;
    int unblocked=0;
    for(int i=0; i< aux; i++) {
        //semaphore->blockedPIDsSize--;
        blockProcess(semaphore->blockedPIDs[i],0);
        semaphore->blockedPIDs[i]=0;
        unblocked++;
    }
    semaphore->blockedPIDsSize = 0;
    release(&(semaphore->lock));
    /*if(unblocked)
        _timer_tick();*/
    return 0;
}


int deleteSemaphore(int id){

    Semaphore *prevSemaphore = findSemaphore(id, 1);
    Semaphore *semaphore = findSemaphore(id,0);
    if(semaphore==NULL || semaphore->blockedPIDsSize>0 )
        return -1;
    if(prevSemaphore == NULL)
        semaphores = semaphore->next;
    else
        prevSemaphore->next=semaphore->next;

    free(semaphore);
    return 0;

}