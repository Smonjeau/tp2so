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

    printf("\n----------------------------------------------------------\n", 0);

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


void pipeLeftProcMediator(int argc, char **argv){
    int fds[2];

    fds[0] = atoi(argv[argc+1]);
    fds[1] = atoi(argv[argc+2]);

    close(1);
    dup(fds[1]);

    close(fds[0]);

    startProcess(argv[argc], argc, argv, argv[0], 1);

    kill(-1);
}


void pipeRightProcMediator(int argc, char ** argv){
    int fds[2];

    fds[0] = atoi(argv[argc+1]);
    fds[1] = atoi(argv[argc+2]);

    close(0);
    dup(fds[0]);

    close(fds[1]);

    startProcess(argv[argc], argc, argv, argv[0], 0);

    kill(-1);
}