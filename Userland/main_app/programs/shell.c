/*---------------------------------------------------------------------------------------------------
|   SHELL.C    |                                                                             	    |
|---------------                                                                                    |
| This windows will display the shell program.														|
| Press ENTER to execute a command.																	|
---------------------------------------------------------------------------------------------------*/

#include <shell_cmds.h>
#include <syscalls.h>
#include <std_lib.h>
#include <std_io.h>
#include <asm_lib.h>

/* --------------------------------------------------------------------------------------------------------------------------
                                        		SHELL DEFINITIONS
------------------------------------------------------------------------------------------------------------------------- */

#define NULL ((void *) 0)

#define MAX_TOKENS 10
#define MAX_TOKEN_LEN 20
#define MAX_CMD_LEN MAX_TOKENS*MAX_TOKEN_LEN

void parseCommand(char *cmdBuff);

void dummy(int argc, char **argv);


/* --------------------------------------------------------------------------------------------------------------------------
                                        	SHELL METHODS
------------------------------------------------------------------------------------------------------------------------- */



void shell(){
    startProcess(dummy, 0, (void *) 0, "dummy", 0); //Necesario en ciertos casos

	forcePipe(0); //Creamos el pipe que comunica fd 0 con teclado
	forcePipe(1); //Creamos el pipe que comunica fd 1 con salida estandar
	

	char cmdBuff[MAX_CMD_LEN] = {0};
	int buffPos = 0;

	printf("\f", 0);

	int fds[2];
	pipe(fds);

	// Las siguientes pruebas funcionan por separado pero porque se envia EOT a mano, para el envio automatico de EOT
	// va a ser necesario distinguir FDs de lectura/escritura, ya que si no no hay manera de saber si quedan productores.
	// No sirve preguntar si queda una sola boca, ya que podrian haber bocas de multiples lectores, como hijos de procesos
	// que aun no la cerraron.
	

	// pipeLeftProc(printHelp, "help", fds);
	// char cc;
	// do{
	// 	read(fds[1], &cc, 1);
	// 	putChar(cc);
	// }while(cc!= 4);
	// close(fds[1]);

	// pipeRightProc(filter, "filter", fds);
	// write(fds[0], "Hola\4", 5);
	// close(fds[0]);


	// La conexion entre los dos procesos no va a funcionar no solo porque falla el envio de EOT, si no por que ademas hay
	// condicion de carrera, ya que si el right process llega antes que el left process, va a querer leer de un pipe que
	// no tiene escritor y por tanto deberia recibir EOT. Notar que para cuando llegue right process, la shell ya va a haber
	// cerrado su boca como escritora, ya que no le compete.


	char c;
	while (1) {
		c = getChar();

		// Handle the chars that are not CR

		if (c == '\b' && buffPos > 0)
			cmdBuff[--buffPos] = 0;
		
		if (c >= 32 && c <= 127 && buffPos < MAX_CMD_LEN)
			cmdBuff[buffPos++] = c;

		putChar(c);


            // Handle the CR char, parse command

		if(c=='\r'){
			if(buffPos==0){
				putChar('\n');
				continue;
			}

			putChar('\n');

			cmdBuff[buffPos] = 0;
			parseCommand(cmdBuff);

			putChar('\n');
			putChar('\n');


            buffPos = 0;
        }
    }

 }
    


/* --------------------------------------------------------------------------------------------------------------------------
                                COMMAND-CHECK METHODS
------------------------------------------------------------------------------------------------------------------------- */

#define MAX_SIMPLE_COMMANDS 2

typedef struct ProcessExec{
	void (*main)(int,char**);
	int argc;
	char argv[MAX_TOKENS][MAX_TOKEN_LEN];
	char name[MAX_TOKEN_LEN];
	int foreground;
} ProcessExec;

ProcessExec processExecs[MAX_SIMPLE_COMMANDS] = {{0}};


int parseSimpleCommand(char *cmdBuff, int n);


void parseCommand(char *cmdBuff) {

	char *simpleCommands[MAX_SIMPLE_COMMANDS] = {0};

	simpleCommands[0] = cmdBuff;

	int simpleCommandsSize=1;
	for (int i=0; cmdBuff[i]; i++){
		if (cmdBuff[i] == '|'){
			cmdBuff[i-1] = 0;	// Trim previous space
			simpleCommands[simpleCommandsSize++] = cmdBuff+i+2; // Skip next space
		}
	}

	
	int processes = 0;

	for(int i=0; i<simpleCommandsSize; i++)
		processes += parseSimpleCommand(simpleCommands[i], processes);

	// for(int i=0; i<processes; i++){
	// 	ProcessExec p = processExecs[i];
	// 	startProcess(p.main, p.argc, p.argv, p.name, 1);
	// }

	ProcessExec p = processExecs[0];
	if(p.main != NULL)
		startProcess(p.main, p.argc, (char **) p.argv, p.name, p.foreground);

}


int createProcessExec(void (*main)(int,char**), int argc, char **argv, char *name, int fg, int n){
	processExecs[n].main = main;
	processExecs[n].argc = argc;
	processExecs[n].foreground = fg;
	
	for(int i=0; i<argc; i++)
		memcpy(processExecs[n].argv[i], argv[i], MAX_TOKEN_LEN);

	memcpy(processExecs[n].name, name, MAX_TOKEN_LEN);

	return 1;
}


int parseSimpleCommand(char *cmdBuff, int n){

	// Separate on tokens

	char *tokens[MAX_TOKENS] = {0};
	
	tokens[0] = cmdBuff;

	int j=1;
	for (int i=0; cmdBuff[i]; i++){
		if (cmdBuff[i] == ' ' || cmdBuff[i] == '\t' || cmdBuff[i] == '\n' 
				|| cmdBuff[i] == '\r' || cmdBuff[i] == 0){
			cmdBuff[i] = 0;
			tokens[j++] = cmdBuff+i+1;
		}
	}

	int fg = 1;
	if(strncmp(tokens[j-1], "&", 2) == 0){
		fg = 0;
		j -= 1;
	}

	// Miscellaneous

	if (strncmp(tokens[0], "help", 5) == 0 && j==1)
		createProcessExec(printHelp, 0, NULL, "help", fg , n);

	else if (strncmp(tokens[0], "divzero", 8) == 0 && j==1)
		createProcessExec(divZeroException, 0, NULL, "testnosync", fg , n);

	else if (strncmp(tokens[0], "invopcode", 10) == 0 && j==1)
		createProcessExec(invOpcodeException, 0, NULL, "testnosync", fg , n);

	else if (strncmp(tokens[0], "time", 5) == 0 && j==1)
		createProcessExec(printTime, 0, NULL, "testnosync", fg , n);

	else if(strncmp(tokens[0], "display", 8) == 0 && j==2)
		createProcessExec(displayImage, 1, tokens, "display", fg , n);


	else if(strncmp(tokens[0], "filter", 7) == 0 && j == 1)
		createProcessExec(filter, 0, NULL, "filter", fg , n);

	else if(strncmp(tokens[0], "cat", 4) == 0 && j==1)
		createProcessExec(cat, 0, NULL, "cat", fg , n);
	
	else if(strncmp(tokens[0], "wc", 3) == 0 && j==1)
		createProcessExec(wc, 0, NULL, "wc", fg , n);


	// CPU management

	else if (strncmp(tokens[0], "cputemp", 8) == 0 && j==1)
		createProcessExec(printCPUTemp, 0, NULL, "cputemp", fg , n);

	else if (strncmp(tokens[0], "cpuinfo", 8) == 0 && j==1)
		createProcessExec(printCPUInfo, 0, NULL, "cpuinfo", fg , n);

	else if (strncmp(tokens[0], "inforeg", 8) == 0 && j==1)
		createProcessExec(printInfoReg, 0, NULL, "inforeg", fg , n);

	else if (strncmp(tokens[0], "storedreg", 10) == 0 && j==1)
		createProcessExec(printStoredReg, 0, NULL, "storedreg", fg , n);


	// Memory management

	else if (strncmp(tokens[0], "memdump", 8) == 0 && j == 2)
		createProcessExec(printMemDump, 1, tokens, "memdump", fg , n);

	else if (strncmp(tokens[0], "mem", 4) == 0 && j==1)
		createProcessExec(printMemStatus, 0, NULL, "mem", fg , n);


	// Process management

	else if (strncmp(tokens[0], "kill", 5) == 0 && j == 2)
		createProcessExec(killProcess, 1, tokens, "kill", fg , n);

	else if (strncmp(tokens[0], "block", 6) == 0 && j == 2)
		createProcessExec(blockProcess, 1, tokens, "block", fg , n);

	else if (strncmp(tokens[0], "nice", 5) == 0 && j == 3)
		createProcessExec(niceProcess, 2, tokens+1, "ps", fg , n);

	else if(strncmp(tokens[0],"ps",3)==0 && j==1)
		createProcessExec(printProcData, 0, NULL, "ps", fg , n);


	// New processes

	else if(strncmp(tokens[0], "line", 5) == 0 && j==1)	
		createProcessExec(line, 0, NULL, "line", fg , n);

	else if (strncmp(tokens[0], "loop", 5) == 0 && j==1)
		createProcessExec(loop, 0, NULL, "loop", fg , n);
	else if(strncmp(tokens[0],"phylo",6)==0 && j==1)
		createProcessExec(phylo,0,NULL,"phyloAdmin",fg,n);


	//Sync

	else if (strncmp(*tokens, "sem", 4) == 0 && j==1)
		createProcessExec(printSemStatus, 0, NULL, "sem", fg , n);

	else if(strncmp(tokens[0], "pipe", 5) == 0 && j==1)
		createProcessExec(printPipeInfo, 0, NULL, "pipe", fg , n);


	//Testing

	else if(strncmp(tokens[0],"testmm",7) == 0 && j==1)
		createProcessExec(test_mm, 0, NULL, "testmm", fg , n);

	else if(strncmp(tokens[0],"testproc",7) == 0)
		createProcessExec(test_proc, 0, NULL, "testproc", fg , n);

	else if(strncmp(tokens[0],"testproc",7) == 0 && j==1)
		createProcessExec(test_proc, 0, NULL, "testproc", fg , n);

	else if(strncmp(tokens[0],"testprio",7) == 0 && j==1)
		createProcessExec(test_prio, 0, NULL, "testprio", fg , n);

	else if(strncmp(tokens[0],"testsync",7) == 0 && j==1)
		createProcessExec(test_sync, 0, NULL, "testsync", fg , n);

	else if(strncmp(tokens[0],"testnosync",7) == 0 && j==1)
		createProcessExec(test_no_sync, 0, NULL, "testnosync", fg , n);

	else if(strncmp(tokens[0], "clear", 6) == 0 && j==1){
		putChar('\f');
		return 0;
	}

	else{
		printWarning();
		return 0;
	}

	return 1;
	
}

    
/* --------------------------------------------------------------------------------------------------------------------------
                                        	OTHER METHODS
------------------------------------------------------------------------------------------------------------------------- */

void dummy(int argc, char **argv) {
    printf("Hola soy dummy", 0);
    while(1)
        halt();
}