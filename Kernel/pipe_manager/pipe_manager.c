#include <pipe_manager.h>
#include <process_manager.h>
#include <mem_manager.h>
#include <semaphores.h>
#include <interrupts.h>
#include <keyboard_driver.h>
#include <screen_driver.h>
#include <video_lib.h>
#include <lib.h>
#define READ 0
#define WRITE 1
static int semaphore_id = 0;

pipe pipeList = NULL;

extern pipe stdinPipe;

void addToPipeList(pipe new) {

    if(new == NULL)
        return;

    if(pipeList == NULL) {
        pipeList = new;
    } else {
        pipe aux = pipeList;
        while(aux->nextPipe != NULL)
            aux = aux->nextPipe;
        aux->nextPipe = new;
    }

}


void removeFromPipeList(pipe pipe_rem) {

    if(pipe_rem == NULL || pipeList == NULL)
        return;

    pipe aux = pipeList;
    while(aux->nextPipe != NULL && aux->nextPipe != pipe_rem)
        aux = aux->nextPipe;
        
    if(aux == pipe_rem)
        pipeList = pipe_rem->nextPipe; //Era el primer pipe
    else if(aux->nextPipe == pipe_rem)
        aux->nextPipe = pipe_rem->nextPipe;

}


int initializePipe(pipe new, int isStdio) {

    static int pipeId = 0;

    if(new == NULL)
        return 0;

    new->id = pipeId++;
    new->index_w = 0;
    new->index_r = 0;
    new->open_ports = 2;
    new->write_bytes_sem = semaphore_id++;
    new->read_bytes_sem = semaphore_id++;
    new->nextPipe = NULL;
    new->isStdio = isStdio;
    new->open_read_ports = 1;
    new->open_write_ports = 1;
    
    addToPipeList(new);

    openSemaphore(new->write_bytes_sem, PIPE_SIZE);
    openSemaphore(new->read_bytes_sem, 0);

    return 1;

}


int create_pipe(int fds[2]) {

    pipe new = malloc(sizeof(struct pipe));

    if(initializePipe(new, -1))
        return assign_pipe_to_pcb(fds, new);

    return -1;
    
}


int create_force_pipe(int fd) {

    if(is_fd_free(fd) == 0 || fd < 0 || fd >= MAX_PIPES)
        return -1;

    pipe new = malloc(sizeof(struct pipe));

    if(initializePipe(new, fd) == 1) {

        // Assign one end of pipe to running process

        if(assign_pipe_to_pcb_forced(fd, new) == -1)
            return -1;

        // If STDIN assign other end to keyboard
        if(fd == 0)
            stdinPipe = new;

        // If STOUD it wont have another end, the pipe will be unbuffered
        // and as soon as data arrives it will be printed to the screen

        return 0;
    }

    return -1;   
    
}


void close_port(int fd) {
    close_fd(fd);
}



void copyPipeInfoToBuffer(char * buffer, pipe aux) {

    strcat("ID: ", buffer);
    itoa(aux->id, buffer + strlen(buffer), 10, -1);

    strcat("\nOpen ends: ", buffer);
    itoa(aux->open_ports, buffer + strlen(buffer), 10, -1);
  
    strcat("\nWritten bytes: ", buffer);
    itoa(aux->index_w, buffer + strlen(buffer), 10, -1);
    strcat(" - Read bytes: ", buffer);
    itoa(aux->index_r, buffer + strlen(buffer), 10, -1);

    strcat("\nSem write ID: ", buffer);
    itoa(aux->write_bytes_sem, buffer + strlen(buffer), 10, -1);
    strcat(" - Sem read ID: ", buffer);
    itoa(aux->read_bytes_sem, buffer + strlen(buffer), 10, -1);

    strcat("\nProc bloqueados: ", buffer);
    
    //Primero reviso el sem write
    char buff[20];
    getBlockedProc(buff, aux->write_bytes_sem);

    strcat("\n\n", buffer);

}


void pipeInfo(char * buffer) {

    if(buffer == NULL)
        return;

    pipe aux = pipeList;
    strcpy("", buffer);

    while(aux != NULL) {
        copyPipeInfoToBuffer(buffer, aux);
        aux = aux->nextPipe;
    }

}


void free_pipe_if_empty(pipe pipe) {

	if(pipe->open_ports == 0) {

	    if(closeSemaphore(pipe->read_bytes_sem) == -1 || closeSemaphore(pipe->write_bytes_sem) == -1) {
            return;
        } else {
            removeFromPipeList(pipe);
            free(pipe);
        }

	}
}


int pipe_write(int fd, char * buffer, int bytes) {

    pipe pipe = findPipe(fd);

    return pipe_write_nofd(pipe, buffer, bytes); //Reutilizamos la funcion

}


int pipe_write_nofd(pipe pipe, char * buffer, int bytes) {

    if(pipe == NULL)
        return -1;

    // STDOUT will behave unbuffered, writing directly to screen, without a consumer process
    if(pipe->isStdio == STDOUT){
        print(buffer);

        // Update indexes for information purposes
        pipe->index_w += bytes;
        pipe->index_r += bytes;

        return bytes;
    }

    int i;
    for(i = 0; i < bytes && buffer[i] != 0; i++) {
        //if(buffer[i] != 0) {
            waitSemaphore(pipe->write_bytes_sem);

            pipe->buffer[pipe->index_w++ % PIPE_SIZE] = buffer[i];

            postSemaphore(pipe->read_bytes_sem);
        //}


        /*if(buffer[i] == 0)
            break;*/
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
    return pipe_read_nofd(pipe, buffer, bytes); //Reutilizamos la funcion

}


int pipe_read_nofd(pipe pipe, char * buffer, int bytes) {
 
    if(pipe == NULL)
        return -1;

    if(pipe == stdinPipe && ! hasForeground()){
        blockProcess(getPID(), 1);
        return -1;
    }

    if(pipe->open_write_ports == 0 && bytes > 0) {
        buffer[0] = EOT;
        return 0;
    }

    int i;
    for(i = 0; i < bytes; i++) {
        waitSemaphore(pipe->read_bytes_sem);
        buffer[i] = pipe->buffer[pipe->index_r++ % PIPE_SIZE];
        postSemaphore(pipe->write_bytes_sem);
        if(pipe->buffer[(pipe->index_r-1) % PIPE_SIZE] == 0)
            break;
    }

    return 0;

}