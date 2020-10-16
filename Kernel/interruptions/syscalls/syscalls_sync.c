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

int sysPipe(int * fds) {
	return create_pipe(fds);
}

void sysClosefd(int fd) {
	close_port(fd);
}
int sysPipeWrite(int fd, char * buffer, int bytes) {
    return pipe_write(fd,buffer, bytes);
}

int sysPipeRead (int fd, char * buffer, int bytes) {
    return pipe_read (fd,buffer, bytes);
}