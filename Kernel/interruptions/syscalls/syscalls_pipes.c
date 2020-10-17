#include <pipe_manager.h>

int sysWritePipe(pipe pipe,char * buffer, int bytes){
    return pipe_write(pipe,buffer,bytes);
}
int sysReadPipe(pipe pipe, char * buffer, int bytes){
    return pipe_read(pipe,buffer,bytes);
}

int sysCreatePipe(int * fds){
    return create_pipe(fds);
}

