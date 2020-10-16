#ifndef PIPE__MANAGER__H
#define PIPE__MANAGER__H
#define PIPE_SIZE 255
#include <semaphores.h>

typedef struct pipe {
    int lock;
    int index_w;
    int index_r;
    int write_bytes_sem;
    int read_bytes_sem;
    char buffer [PIPE_SIZE];
    int open_ports; //Esto es para que el pipe sepa que puede liberar la memoria pedida si no hay más procesos usandolo.
} * pipe;

int pipe_write(pipe pipe,char * buffer, int bytes);

int pipe_read (pipe pipe,char * buffer, int bytes);

int create_pipe (int * fds);

void close_port(int fd);

void free_pipe_if_empty(pipe pipe);


#endif