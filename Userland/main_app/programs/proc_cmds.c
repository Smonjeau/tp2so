#include <syscalls.h>
#include <std_lib.h>
#include <shell_cmds.h>


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

void pipeLeftProcMediator(int argc, char * * argv) {
    int fds[2];
    fds[0] = atoi(argv[0]);
    fds[1] = atoi(argv[1]);
    char * name = argv[2];
    void (* main) (int, char **) = (void (*) (int, char **)) argv[3];

    close(1); //Solo leo de stdin
    close(fds[0]); //Solo escribo en el pipe
    dup(fds[1]); //La escritura del pipe se pone en lugar del stdout

    startProcess(main, 4, arg_vals, "left_proc_mediator", 1); //Ojo que despues hay que cambiar 0 por 1 ////////////////////////////////////


    kill(-1); //El kill me hace los close de todas formas
}
void pipeRightProcMediator(int argc, char * * argv) {
    int fds[2];
    fds[0] = atoi(argv[0]);
    fds[1] = atoi(argv[1]);
    char * name = argv[2];
    void (* main) (int, char **) = (void (*) (int, char **)) argv[3];

    close(0); //Solo escribo a stdout
    close(fds[1]); //Solo leo del pipe
    dup(fds[0]); //La lectura del pipe se pone en lugar del stdin

    startProcess(main, 4, arg_vals, "right_proc_mediator", 1); //Ojo que despues hay que cambiar 0 por 1 ////////////////////////////////////


    kill(-1); //El kill me hace los close de todas formas
}

/* Antes de llamar a esta serie de funciones se debe crear pipe y enviar fds por aca*/
/* Al finalizar debe cerrar fds*/
void pipeLeftProc(void (* main) (int, char **), char * name, int fds[2]) {
    //POR AHORA SOLO FOREGRAUND
    //Todavía soy la shell
    char fd1[3]; //buffer para guardar fds[0] como string
    char fd2[3]; //buffer para guardar fds[1] como string
    itoa(fds[0], fd1, 10, -1);
    itoa(fds[1], fd2, 10, -1);
    char * arg_vals[2] = {fd1, fd2, name, (char (*) (int, char **)) main};
    startProcess(main, 4, arg_vals, "left_proc_mediator", 1); //Ojo que despues hay que cambiar 0 por 1 ////////////////////////////////////

    //La shell se bloquea hasta que el proceso de la izquierda termina de escribir y hace exit
}
void pipeRightProc(void (* main) (int, char **), char * name, int fds[2]) {
    //POR AHORA SOLO FOREGRAUND
    //Todavía soy la shell
    char fd1[3]; //buffer para guardar fds[0] como string
    char fd2[3]; //buffer para guardar fds[1] como string
    itoa(fds[0], fd1, 10, -1);
    itoa(fds[1], fd2, 10, -1);
    char * arg_vals[2] = {fd1, fd2, name, (char (*) (int, char **)) main};
    startProcess(main, 4, arg_vals, "right_proc_mediator", 1); //Ojo que despues hay que cambiar 0 por 1 ////////////////////////////////////

    //La shell se bloquea hasta que el proceso de la izquierda termina de escribir y hace exit
}
void pipeLeftProc(void (* main) (int, char **)) {
    //POR AHORA SOLO FOREGRAUND    
}

int startProcess(void *main, int argc, char **argv, char * name, int bg);