/*---------------------------------------------------------------------------------------------------
|   SYSCALLS_PROCESS.C    |                                                                             |
|--------------------                                                                               |
| These functions implement syscalls related to processes management. 	                                    |
---------------------------------------------------------------------------------------------------*/

#include <process_manager.h>

/* ---------------------------------------------------------------------------------------------------------------------------
                                                    START_PROCESS
--------------------------------------------------------------------------------------------------------------------------- */

int sysStartProcess(void *main, int argc, char **argv, char * name){

    return createProcessContext(argc, argv, main,name);

}

void sysPS(void * buffer, int * procCant) {
	ps(buffer, procCant);
}

void sysKill(int pid){
    killProcess(pid);
}