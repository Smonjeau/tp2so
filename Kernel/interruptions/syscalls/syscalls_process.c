/*---------------------------------------------------------------------------------------------------
|   SYSCALLS_PROCESS.C    |                                                                             |
|--------------------                                                                               |
| These functions implement syscalls related to processes management. 	                                    |
---------------------------------------------------------------------------------------------------*/

#include <process_manager.h>
#include <interrupts.h>
#include <lib.h>

/* ---------------------------------------------------------------------------------------------------------------------------
                                                    START_PROCESS
--------------------------------------------------------------------------------------------------------------------------- */

// typedef void (*ProcessMain)(int argc, char **argv);

// void mainWrapper(int argc, char **argv){

//     ProcessMain main = (ProcessMain) argv[0];

// 	main(argc, argc>0 ? argv+1 : NULL);

// 	killProcess(-1);

// }

int sysStartProcess(void *main, int argc, char **argv, char * name, int foreground){

    // int wargc = argc+1;

    // char *wargv[wargc];
    // wargv[0] = main;
    
    // if(argv != NULL)
    //     memcpy(wargv+1, argv, argc);

    return createProcessContext(argc, argv, main, name, foreground);

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

int sysDup(int fd) {
    return dup_fd(fd);
}

void sysClosefd(int fd) {
	close_port(fd);
}