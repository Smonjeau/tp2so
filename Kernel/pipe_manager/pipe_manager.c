#include <pipe_manager.h>
#include <process_manager.h>
#include <mem_manager.h>
#define READ 0
#define WRITE



int create_pipe (int * fds){
	pipe new = malloc(sizeof(struct pipe));
	if(!new)
		return -1; //:((
	new->open_ports=2;
	new->write_bytes_sem = createSemaphore(PIPE_SIZE);
	new->read_bytes_sem = createSemaphore(0);
	return assign_pipe_to_pcb(fds,new);

}

void close_port(int fd){
	close_fd(fd);
}

void free_pipe_if_empty(pipe pipe){
	if(pipe->open_ports==0){
		deleteSemaphore(pipe->read_bytes_sem);
		deleteSemaphore(pipe->write_bytes_sem);
		free(pipe);
	
	}
}

int pipe_write(pipe pipe,char * buffer, int bytes){
	for(int i=0; i<bytes;i++){
		//Esperar a que haya para escribir
		waitSemaphore(pipe->write_bytes_sem);

				

	}
}