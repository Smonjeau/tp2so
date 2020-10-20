#include <syscalls.h>
#include <std_lib.h>
#include <shell_cmds.h>

#define NULL (void *)0


void printProcData(int argc, char **argv){

	char * buffer = malloc(90 * 40);
    if(buffer == (void *) 0)
		return;

    printf("----------------------------------------------------------\n", 0);

    ps(buffer);    
    printf(buffer, 0);
    free(buffer);

    printf("----------------------------------------------------------\n", 0);

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

    printf("----------------------------------------------------------\n", 0);

	char * buffer = malloc(90 * 40);
    if(buffer == (void *) 0)
		kill(-1);

    pipeInfo(buffer);    
    printf(buffer, 0);
    free(buffer);

    printf("\n----------------------------------------------------------", 0);

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

    printf("----------------------------------------------------------\n", 0);

    while(i<qty){
        sem=*(buffer + i++);
        printf("ID: %d\n", 1, sem.id);
        printf("Value: %d\n", 1, sem.value);
        printf("Blocked processes: %d\n", 1, sem.blockedPIDsSize);
        
       	if(sem.blockedPIDsSize>0)
            printf("The blocked processes are:",0);
        for(int j=0;j<sem.blockedPIDsSize;j++)
            printf(" %d ",1,sem.blockedPIDs[j]);

        printf("\n", 0);
    }

    printf("\n----------------------------------------------------------", 0);

    free(buffer);

    kill(-1);

}


typedef void (*ProcMain)(int,char**);


char largv[3][20];
ProcMain laddr;

void pipeLeftProcMediator(int argc, char **argv) {
    int fds[2];

    fds[0] = atoi(largv[0]);
    fds[1] = atoi(largv[1]);

    // printf("LP: %d -> %d\n", 2, fds[0], fds[1]);

    char * name = largv[2];

    close(1);
    dup(fds[0]);

    close(fds[1]);

    startProcess(laddr, 0, NULL, name, 1);

    kill(-1);
}


char rargv[4][20];
ProcMain raddr;

void pipeRightProcMediator(int argc, char ** argv) {
    int fds[2];

    fds[0] = atoi(rargv[0]);
    fds[1] = atoi(rargv[1]);

    // printf("RP: %d -> %d\n", 2, fds[0], fds[1]);

    char * name = rargv[2];

    close(0);
    dup(fds[1]);

    close(fds[0]);

    startProcess(raddr, 0, NULL, name, 0);

    kill(-1);
}


void pipeLeftProc(ProcMain main, char * name, int fds[2]) {

    char fd1[3];
    char fd2[3];

    itoa(fds[0], fd1, 10, -1);
    itoa(fds[1], fd2, 10, -1);

    strcpy(fd1, largv[0]);
    strcpy(fd2, largv[1]);
    strcpy(name, largv[2]);
    laddr = main;
    
    startProcess(pipeLeftProcMediator, 4, (char **) largv, "left_proc_mediator", 1);

}


void pipeRightProc(ProcMain main, char * name, int fds[2]) {

    char fd1[3];
    char fd2[3];

    itoa(fds[0], fd1, 10, -1);
    itoa(fds[1], fd2, 10, -1);

    strcpy(fd1, rargv[0]);
    strcpy(fd2, rargv[1]);
    strcpy(name, rargv[2]);
    raddr = main;

    startProcess(pipeRightProcMediator, 4, (char **) rargv, "right_proc_mediator", 0);

}
