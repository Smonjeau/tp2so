#include <pipe_manager.h>
#include <process_manager.h>
#include <mem_manager.h>
#include <semaphores.h>
#include <interrupts.h>
#include <keyboard_driver.h>
#include <screen_driver.h>
#define READ 0
#define WRITE 1
static int semaphore_id = 0;
int create_pipe(int  fds [2]) {

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

int create_force_pipe(int fd) {
    if(is_fd_free(fd) == 0 || fd < 0 || fd >= MAX_PIPES)
        return -1;
    //Tengo el fd indicado libre
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
    if(assign_pipe_to_pcb_forced(fd, new) == -1)
        return -1;
    switch(fd) {
        case 0:
            //STDIN es el teclado.
            //Asignamos la segunda boca del pipe al keyboardDriver
            assignKeyboardPipe(new);
            break;
    }
    return 0;
}

void close_port(int fd) {
    close_fd(fd);
}

void drawLine3(){
    static int c=0;
    c+=1;

        for(int x=0; x<1024; x++)
            draw(x,c*220,0xFFFFFF);

}
void free_pipe_if_empty(pipe pipe) {
	if(pipe->open_ports == 0) {

	    if(deleteSemaphore(pipe->read_bytes_sem) == -1 || deleteSemaphore(pipe->write_bytes_sem) == -1)
            drawLine3();
        else
            free(pipe);
	}
}

int pipe_write(int fd, char * buffer, int bytes) {

    pipe pipe = findPipe(fd);
    return pipe_write_nofd(pipe, buffer, bytes); //Reutilizamos la funcion
}
int pipe_write_nofd(pipe pipe, char * buffer, int bytes) {
    if(pipe == NULL)
        return -1;
    int i;
    for(i = 0; i < bytes; i++) {
        waitSemaphore(pipe->write_bytes_sem);
        pipe->buffer[pipe->index_w++ % PIPE_SIZE] = buffer[i];

        postSemaphore(pipe->read_bytes_sem);

        if(buffer[i] == 0)
            break;
        /*Esto es porque se puede dar el caso que el write acapare toda la cpu --> Si tenía 2 ticks al arrancar
        y libera procesos al hacer post, llama al scheduler pero sabemos que lo van a volver a elegir, entonces como despúes ya
        no se llama al scheduler(no desbloqueamos a nadie), el write acapara toda la cpu, entonces le pongo este límite*/
        /*if(pipe->index_w % 100 == 0) {
            postSemaphore(pipe->read_bytes_sem);
            _timer_tick();

        }*/
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