/*---------------------------------------------------------------------------------------------------
|   SYSCALLS_SYNC.C    |                                                                            |
|--------------------                                                                               |
| These functions implement syscalls related to process synchronization.   	                        |
---------------------------------------------------------------------------------------------------*/

#include <semaphores.h>
#include <pipe_manager.h>

/* ---------------------------------------------------------------------------------------------------------------------------
                                                    CREATE_SEM
--------------------------------------------------------------------------------------------------------------------------- */

int sysCreateSemaphore( int id,int initValue){
    return openSemaphore(id, initValue);
}

int sysWaitSemaphore(int id){
    return waitSemaphore(id);
}

int sysPostSemaphore(int id){
    return postSemaphore(id);
}

int sysDeleteSemaphore(int id){
    return closeSemaphore(id);
}

void sysSemStatus(void * buffer, int * qty){
    semStatus(buffer,qty);
}

int sysPipe(int * fds) {
	return create_pipe(fds);
}

void sysPipeInfo(char * buffer) {
	pipeInfo(buffer);
}

int sysForcePipe(int fd) {
	return create_force_pipe(fd);
}

void sysClosefd(int fd) {
	close_port(fd);
}

int sysPipeRead (int fd, char * buffer, int bytes) {
    return pipe_read (fd,buffer, bytes);
}

