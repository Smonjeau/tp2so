#include <pipe_manager.h>
#include <process_manager.h>
#include <mem_manager.h>
#define READ 0
#define WRITE 1
static int semaphore_id = 0;
int create_pipe(int * fds) {

    pipe new = malloc(sizeof(struct pipe));
    if(new == NULL)
        return -1;
    new->index_w = 0;
    new->index_r = 0;
    new->open_ports = 2;
    new->write_bytes_sem = semaphore_id++;
    new->read_bytes_sem = semaphore_id++;
    createSemaphore(new->write_bytes_sem, PIPE_SIZE);
    createSemaphore(new->read_bytes_sem, 0);
    return assign_pipe_to_pcb(fds, new);
}

void close_port(int fd) {
    close_fd(fd);
}

void free_pipe_if_empty(pipe pipe) {
	if(pipe->open_ports == 0) {
	    deleteSemaphore(pipe->read_bytes_sem);
        deleteSemaphore(pipe->write_bytes_sem);
        free(pipe);
	}
}

int pipe_write(int fd, char * buffer, int bytes) {
    pipe pipe = findPipe(fd);
    if(pipe == NULL)
        return -1;
    int i;
    for(i = 0; i < bytes; i++) {
        waitSemaphore(pipe->write_bytes_sem);
        pipe->buffer[pipe->index_w++ % PIPE_SIZE] = buffer[i];
        postSemaphore(pipe->read_bytes_sem);
        if(buffer[i] == 0)
            break;
    }
    return i;
}
int pipe_read(int fd, char * buffer, int bytes) {
    pipe pipe = findPipe(fd);
    if(pipe == NULL)
        return -1;
    int i;
    for(i = 0; i < bytes; i++) {
        waitSemaphore(pipe->read_bytes_sem);
        buffer[i] = pipe->buffer[pipe->index_r++ % PIPE_SIZE];
        postSemaphore(pipe->write_bytes_sem);
        if(pipe->buffer[(pipe->index_r-1) % PIPE_SIZE] == 0)
            break;
    }

    return i;
}