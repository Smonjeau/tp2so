/*---------------------------------------------------------------------------------------------------
|   SHELL.C    |                                                                             	    |
|----------------------                                                                             |
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

void testKill(){
	for(int i=0; i<999; i++);
	kill(-1);
	printf("Sigo vivo\n", 0);
	while(1);
}


void shell(){

    startProcess(dummy, 0, (void *) 0, "dummy"); //Necesario en ciertos casos

	forcePipe(0); //Creamos el pipe que comunica fd 0 con teclado
	forcePipe(1); //Creamos el pipe que comunica fd 1 con salida estandar

	char cmdBuff[MAX_CMD_LEN] = {0};
	int buffPos = 0;

	printf("\f", 0);

	// startProcess(testKill, 0, NULL, "testKill");

	// startProcess(test_proc, 0, NULL, "test_proc");

	char c;
	while (1){
		c = getChar();

		// Handle the chars that are not CR

		if (c == '\b' && buffPos > 0)
			cmdBuff[--buffPos] = 0;
		
		if (c >= 32 && c <= 127 && buffPos < MAX_CMD_LEN)
			cmdBuff[buffPos++] = c;

		putChar(c);

		// Handle the CR char, parse command

		if (c == '\r'){

			if(buffPos==0){
				putChar('\n');
				continue;
			}

			putChar('\n');

			cmdBuff[buffPos] = 0;
			parseCommand(cmdBuff);

			putChar('\n');

			buffPos = 0;
		}
	}
}


/* --------------------------------------------------------------------------------------------------------------------------
                                COMMAND-CHECK METHODS
------------------------------------------------------------------------------------------------------------------------- */

void parseCommand(char *cmdBuff){

	// Separate on tokens

	char *tokens[MAX_TOKENS] = {0};
	tokens[0] = cmdBuff;

	int j=1;
	for (int i=0; cmdBuff[i]; i++){
		if (cmdBuff[i] == ' ' || cmdBuff[i] == '\t' || cmdBuff[i] == '\n' || cmdBuff[i] == '\r' || cmdBuff[i] == 0){
			cmdBuff[i] = 0;
			tokens[j++] = cmdBuff+i+1;
		}
	}

	// Miscellaneous

	if(strncmp(tokens[0], "help", 5) == 0)
		printHelp();

	else if(strncmp(tokens[0], "divzero", 8) == 0)
		divZeroException();

	else if(strncmp(tokens[0], "invopcode", 10) == 0)
		invOpcodeException();

	else if(strncmp(tokens[0], "time", 5) == 0)
		printTime();

	else if(strncmp(tokens[0], "display", 8) == 0 && j==2)
		displayImage(tokens[1], 20, 200);

	else if(strncmp(tokens[0], "filter", 7) == 0)
		startFilter();
	
	else if(strncmp(tokens[0], "clear", 6) == 0)
		putChar('\f');


	// CPU management

	else if(strncmp(tokens[0], "cputemp", 8) == 0)
		printCPUTemp();

	else if(strncmp(tokens[0], "cpuinfo", 8) == 0)
		printCPUInfo();

	else if(strncmp(tokens[0], "inforeg", 8) == 0)
		printInfoReg();

	else if(strncmp(tokens[0], "storedreg", 10) == 0)
		printStoredReg();
	

	// Memory management

	else if(strncmp(tokens[0], "memdump", 8) == 0 && j==2)
		printMemDump(tokens[1]);
	
	else if(strncmp(tokens[0], "mem", 4) == 0)
		printMemStatus();

	
	// Process management
	
	else if(strncmp(tokens[0], "ps", 3) == 0)
		printProcData();

	else if(strncmp(tokens[0], "kill", 5) == 0 && j==2)
		killProcess(tokens[1]);

	else if(strncmp(tokens[0], "block", 6) == 0 && j==2)
		blockProcess(tokens[1]);

	else if(strncmp(tokens[0], "nice", 5) == 0 && j==3)
		niceProcess(tokens[1], tokens[2]);
	

	// New processes

	else if(strncmp(tokens[0], "line", 5) == 0)	
		startProcess(line, 0, NULL, "line");

	else if(strncmp(tokens[0], "loop", 5) == 0)
		startProcess(loop, 0, NULL, "loop");


	//Sync
	else if(strncmp(*tokens,"sem",4)==0)
	    printSemStatus();

	else if(strncmp(tokens[0], "pipe", 5) == 0)
		printPipeInfo();


	//Testing
	else if(strncmp(tokens[0],"testmm",7) == 0)
		startProcess(test_mm, 0, NULL, "testmm");

	else if(strncmp(tokens[0],"testproc",7) == 0)
		startProcess(test_proc, 0, NULL, "testproc");

	else if(strncmp(tokens[0],"testsync",7) == 0)
		startProcess(test_sync, 0, NULL, "testsync");

	// Command not found
	else
		printWarning();
	
}


/* --------------------------------------------------------------------------------------------------------------------------
                                        	OTHER METHODS
------------------------------------------------------------------------------------------------------------------------- */

void dummy(int argc, char **argv) {
    while(1)
        halt();
}