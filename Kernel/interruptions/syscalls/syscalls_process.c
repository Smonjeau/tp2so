/*---------------------------------------------------------------------------------------------------
|   SYSCALLS_PROCESS.C    |                                                                             |
|--------------------                                                                               |
| These functions implement syscalls related to processes management. 	                                    |
---------------------------------------------------------------------------------------------------*/

#include <process_manager.h>
#include <interrupts.h>

/* ---------------------------------------------------------------------------------------------------------------------------
                                                    START_PROCESS
--------------------------------------------------------------------------------------------------------------------------- */

int sysStartProcess(void *main, int argc, char **argv, char * name, int bg){
    return createProcessContext(argc, argv, main, name, bg);
}

void sysPS(char * buffer) {
    ps(buffer);
}

void sysKill(int pid){
    killProcess(pid);
}

int sysGetPid(){
    return getPID();
}

void sysBlock(int pid) {
	blockProcess(pid,1);
}

void sysNice(int pid, int priority) {
	niceProcess(pid, priority);
}