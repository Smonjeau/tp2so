#include <syscalls.h>
#include <std_lib.h>
#include <shell_cmds.h>


#define MAX_BLOCKED_PIDS 20


void printProcData(){

	char * buffer = malloc(90 * 40);
    if(buffer == (void *) 0)
		return;

    ps(buffer);    
    printf(buffer, 0);
    free(buffer);

}


void killProcess(char * pid){

	int _pid = atoi(pid);
    printf("pepito\n",0 );
	if(_pid==-1){
		printf("Argument must be a pid. Use ps to see processes\n", 0);
		return;
	}
    else if(_pid==0){
        printf("Cannot kill shell process\n", 0);
        return;

    } else if(_pid==1){
        printf("Dont kill the dummy proccess", 0);
        return;
    }

	kill(_pid);

}


void blockProcess(char * pid){

	int _pid= atoi(pid);

	if(_pid==-1){
		printf("Argument must be a pid. Use ps to see processes\n", 0);
		return;
	} else if(_pid==0){
		printf("Can not block the shell", 0);
		return;
	} else if(_pid==1){
        printf("Dont block the dummy proccess", 0);
        return;
    }

	block(_pid);

}


void printPipeInfo() {

	char * buffer = malloc(90 * 40);
    if(buffer == (void *) 0)
		return;

    pipeInfo(buffer);    
    printf(buffer, 0);
    free(buffer);
	
}


void niceProcess(char * pid, char * priority) {

	int _pid = atoi(pid);
	int _prio = atoi(priority);

	if(_pid==-1){
		printf("Argument must be a pid. Use ps to see processes\n", 0);
		return;
	}

	nice(_pid, _prio);

}



typedef struct Semaphore{
    int id;
    int value;
    int blockedPIDs[MAX_BLOCKED_PIDS];
    int blockedPIDsSize;
    int lock;
    struct Semaphore * next;
}  Semaphore;

void printSemStatus(){

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
    
}