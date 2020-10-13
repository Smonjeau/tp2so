/*---------------------------------------------------------------------------------------------------
|   SYSCALLS_SYNC.C    |                                                                            |
|--------------------                                                                               |
| These functions implement syscalls related to process synchronization.   	                        |
---------------------------------------------------------------------------------------------------*/

#include <semaphores.h>

/* ---------------------------------------------------------------------------------------------------------------------------
                                                    CREATE_SEM
--------------------------------------------------------------------------------------------------------------------------- */

int sysCreateSemaphore(int id, int initValue){
    return createSemaphore(id, initValue);
}

int sysWaitSemaphore(int id){
    return waitSemaphore(id);
}

int sysPostSemaphore(int id){
    return postSemaphore(id);
}

int sysDeleteSemaphore(int id){
    return deleteSemaphore(id);
}