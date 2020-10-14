#ifndef PIPE__MANAGER__H
#define PIPE__MANAGER__H
#define PIPE_SIZE 255
typedef struct pipe {
    int lock;
    int write_bytes;
    int read_bytes;
    char buffer [PIPE_SIZE];
    int proc_qty; //Esto es para que el pipe sepa que puede liberar la memoria pedida si no hay más procesos usandolo.
}*pipe;

int pipe_write(pipe pipe,char * buffer, int bytes);

int pipe_read (pipe pipe,char * buffer, int bytes);


#endif