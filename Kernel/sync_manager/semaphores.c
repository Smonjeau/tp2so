// #include <mem_manager.h>
// #include <lib.h>
// #include <process_manager.h>

// #define NULL ((void *) 0)
// #define MAX_BLOCKED_PIDS 20

// typedef struct Semaphore{
//     int id;
//     int value;
//     int blockedPIDs[MAX_BLOCKED_PIDS];
//     int blockedPIDsSize;
//     Semaphore * next;
// } Semaphore;

// Semaphore * semaphores = NULL;


// int createSemaphore(int id, int initValue){

//     Semaphore *newSem = malloc(sizeof(Semaphore));
//     if(newSem == NULL)
//         return -1;

//     newSem->id = id;
//     newSem->value = initValue;
//     memset(newSem->blockedPIDs, 0, MAX_BLOCKED_PIDS);
//     newSem->blockedPIDsSize = 0;
//     newSem->next = NULL;

//     Semaphore *lastSem = semaphores;
//     if(lastSem == NULL){
//         lastSem = newSem;
//     }else{
//         while(lastSem->next != NULL)
//             lastSem = lastSem->next;
//         lastSem->next = newSem;
//     }

// }


// Semaphore * findSemaphore(int id){

//     if(semaphores == NULL)
//         return NULL;
    
//     Semaphore *semaphore = semaphores;
//     while(semaphore!=NULL && semaphore->id != id)
//         semaphore = semaphore->next;

//     return semaphore;

// }


// int waitSemaphore(int id){

//     Semaphore *semaphore = findSemaphore(id);
//     if(semaphore == NULL)
//         return -1;

//     if(semaphore->value > 0){
//         semaphore->value -= 1;
//         return 0;
//     }

//     int currPid = getPID();
//     blockProcess(currPid);
//     semaphore->blockedPIDs[semaphore->blockedPIDsSize++] = currPid;

// }


// int postSemaphore(int id){

//     Semaphore *semaphore = findSemaphore(id);
//     if(semaphore == NULL)
//         return -1;

//     if(semaphore->value == 0){
//         semaphore->blockedPIDs;
//     }

//     semaphore->value += 1;

// }