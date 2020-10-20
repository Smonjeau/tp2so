#include <syscalls.h>
#include <std_lib.h>
#include <shell_cmds.h>

#define NULL (void *)0


void printProcData(int argc, char **argv){

	char * buffer = malloc(90 * 40);
    if(buffer == (void *) 0)
		return;

    ps(buffer);    
    printf(buffer, 0);
    free(buffer);

    kill(-1);

}


void killProcess(int argc, char **argv){

	int _pid = atoi(argv[1]);
	if(_pid==-1){
		printf("Argument must be a pid. Use ps to see processes\n", 0);
	}
    else if(_pid==0){
        printf("Cannot kill shell process\n", 0);

    } else if(_pid==1){
        printf("Dont kill the dummy proccess", 0);
    }else{
        kill(_pid);
    }

    kill(-1);

}


void blockProcess(int argc, char **argv){

	int _pid = atoi(argv[1]);

	if(_pid==-1){
		printf("Argument must be a pid. Use ps to see processes\n", 0);
	} else if(_pid==0){
		printf("Can not block the shell", 0);
	} else if(_pid==1){
        printf("Dont block the dummy proccess", 0);
    }else{
        block(_pid);
    }

    kill(-1);

}


void printPipeInfo(int argc, char **argv) {

	char * buffer = malloc(90 * 40);
    if(buffer == (void *) 0)
		kill(-1);

    pipeInfo(buffer);    
    printf(buffer, 0);
    free(buffer);

    kill(-1);
	
}


void niceProcess(int argc, char **argv) {

	int _pid = atoi(argv[1]);
	int _prio = atoi(argv[2]);

	if(_pid==-1){
		printf("Argument must be a pid. Use ps to see processes\n", 0);
		kill(-1);
	}

	nice(_pid, _prio);

    kill(-1);

}


void printSemStatus(int argc, char **argv){

    Semaphore * buffer = malloc(20*sizeof(struct Semaphore));

    int qty =0;
    semStatus(buffer,&qty);

    Semaphore sem;
    int i=0;

    printf("Semaphores status\n",0);

    while(i<qty){
        sem=*(buffer + i++);
        printf("ID:%d  Value:%d   There are %d processes blocked in this semaphore.\n",3,sem.id,sem.value,sem.blockedPIDsSize);
        
       	if(sem.blockedPIDsSize>0)
            printf("The blocked processes are:",0);
        for(int j=0;j<sem.blockedPIDsSize;j++)
            printf(" %d ",1,sem.blockedPIDs[j]);

    }

    free(buffer);

    kill(-1);

}


typedef void (*ProcMain)(int,char**);


void pipeLeftProcMediator(int argc, char **argv) {
    int fds[2];

    printf(argv[0],0);
    printf(argv[1],0);

    fds[0] = atoi(argv[0]);
    fds[1] = atoi(argv[1]);

    // printf("LPM: %d -> %d\n", 2, fds[0], fds[1]);

    // char * name = argv[2];
    // ProcMain main = (ProcMain) argv[3];

    // close(1);
    // dup(fds[0]);

    // close(fds[1]);

    // startProcess(main, 0, NULL, name, 1);

    kill(-1);
}


void pipeRightProcMediator(int argc, char * * argv) {
    int fds[2];

    fds[0] = atoi(argv[0]);
    fds[1] = atoi(argv[1]);

    // printf("RP: %d -> %d\n", 2, fds[0], fds[1]);

    char * name = argv[2];
    ProcMain main = (ProcMain) argv[3];

    close(0);
    dup(fds[1]);

    close(fds[0]);

    startProcess(main, 0, NULL, name, 1);

    kill(-1);
}


void pipeLeftProc(ProcMain main, char * name, int fds[2]) {

    char fd1[3];
    char fd2[3];

    itoa(fds[0], fd1, 10, -1);
    itoa(fds[1], fd2, 10, -1);

    char * argv[4] = {fd1, fd2, name, (char *) main};
    startProcess(pipeLeftProcMediator, 4, argv, "left_proc_mediator", 1);

}


void pipeRightProc(ProcMain main, char * name, int fds[2]) {

    char fd1[3];
    char fd2[3];

    itoa(fds[0], fd1, 10, -1);
    itoa(fds[1], fd2, 10, -1);

    char * argv[4] = {fd1, fd2, name, (char *) main};
    startProcess(pipeRightProcMediator, 4, argv, "right_proc_mediator", 1);

}