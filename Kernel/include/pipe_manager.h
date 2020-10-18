#ifndef PIPE__MANAGER__H
#define PIPE__MANAGER__H
#define PIPE_SIZE 255
#include <semaphores.h>

typedef struct pipe {
    int index_w;
    int index_r;
    int write_bytes_sem;
    int read_bytes_sem;
    char buffer [PIPE_SIZE];
    int open_ports; //Esto es para que el pipe sepa que puede liberar la memoria pedida si no hay más procesos usandolo.
    struct pipe * nextPipe;
} * pipe;

int pipe_write(int fd, char * buffer, int bytes);

int pipe_write_nofd(pipe pipe, char * buffer, int bytes);

int pipe_read (int fd, char * buffer, int bytes);

int create_pipe (int * fds);

/* Crea un pipe con una unica boca definida por parametro y la otra boca de acuerdo al fd recibido. Usado para teclado y consola de texto*/
int create_force_pipe(int fd);

void close_port(int fd);

void free_pipe_if_empty(pipe pipe);

void pipeInfo(char * buffer);


#endif