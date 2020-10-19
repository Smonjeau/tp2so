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

		if(c=='\r'){
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

char *tokens[MAX_TOKENS] = {0};

void parseCommand(char *cmdBuff) {

	// Separate on tokens
	
	tokens[0] = cmdBuff;

	int j=1;
	for (int i=0; cmdBuff[i]; i++){
		if (cmdBuff[i] == ' ' || cmdBuff[i] == '\t' || cmdBuff[i] == '\n' || cmdBuff[i] == '\r' || cmdBuff[i] == 0){
			cmdBuff[i] = 0;
			tokens[j++] = cmdBuff+i+1;
		}
	}

	int foreground = 1;
	if(strncmp(tokens[j-1], "&", 2) == 0){
		foreground = 0;
		j -= 1;
	}

	// Miscellaneous

	if (strncmp(tokens[0], "help", 5) == 0 && j==1)
		startProcess(printHelp, 0, NULL, "testnosync", foreground);

	else if (strncmp(tokens[0], "divzero", 8) == 0 && j==1)
		startProcess(divZeroException, 0, NULL, "testnosync", foreground);

	else if (strncmp(tokens[0], "invopcode", 10) == 0 && j==1)
		startProcess(invOpcodeException, 0, NULL, "testnosync", foreground);

	else if (strncmp(tokens[0], "time", 5) == 0 && j==1)
		startProcess(printTime, 0, NULL, "testnosync", foreground);

	else if(strncmp(tokens[0], "display", 8) == 0 && j==2)
		startProcess(displayImage, 1, tokens, "display", foreground);


	else if(strncmp(tokens[0], "filter", 7) == 0 && j == 1)
		startProcess(filter, 0, NULL, "filter", foreground);

	else if(strncmp(tokens[0], "cat", 4) == 0 && j==1)
		startProcess(cat, 0, NULL, "cat", foreground);
	
	else if(strncmp(tokens[0], "wc", 3) == 0 && j==1)
		startProcess(wc, 0, NULL, "wc", foreground);

	else if(strncmp(tokens[0], "clear", 6) == 0 && j==1)
		putChar('\f');


	// CPU management

	else if (strncmp(tokens[0], "cputemp", 8) == 0 && j==1)
		startProcess(printCPUTemp, 0, NULL, "cputemp", foreground);

	else if (strncmp(tokens[0], "cpuinfo", 8) == 0 && j==1)
		startProcess(printCPUInfo, 0, NULL, "cpuinfo", foreground);

	else if (strncmp(tokens[0], "inforeg", 8) == 0 && j==1)
		startProcess(printInfoReg, 0, NULL, "inforeg", foreground);

	else if (strncmp(tokens[0], "storedreg", 10) == 0 && j==1)
		startProcess(printStoredReg, 0, NULL, "storedreg", foreground);


	// Memory management

	else if (strncmp(tokens[0], "memdump", 8) == 0 && j == 2)
		startProcess(printMemDump, 1, tokens, "memdump", foreground);

	else if (strncmp(tokens[0], "mem", 4) == 0 && j==1)
		startProcess(printMemStatus, 0, NULL, "mem", foreground);


	// Process management

	else if (strncmp(tokens[0], "kill", 5) == 0 && j == 2)
		startProcess(killProcess, 1, tokens, "kill", foreground);

	else if (strncmp(tokens[0], "block", 6) == 0 && j == 2)
		startProcess(blockProcess, 1, tokens, "block", foreground);

	else if (strncmp(tokens[0], "nice", 5) == 0 && j == 3)
		startProcess(niceProcess, 2, tokens+1, "ps", foreground);

	else if(strncmp(tokens[0],"ps",3)==0 && j==1)
		startProcess(printProcData, 0, NULL, "ps", foreground);


	// New processes

	else if(strncmp(tokens[0], "line", 5) == 0 && j==1)	
		startProcess(line, 0, NULL, "line", foreground);

	else if (strncmp(tokens[0], "loop", 5) == 0 && j==1)
		startProcess(loop, 0, NULL, "loop", foreground);


	//Sync

	else if (strncmp(*tokens, "sem", 4) == 0 && j==1)
		startProcess(printSemStatus, 0, NULL, "sem", foreground);

	else if(strncmp(tokens[0], "pipe", 5) == 0 && j==1)
		startProcess(printPipeInfo, 0, NULL, "pipe", foreground);


	//Testing

	else if(strncmp(tokens[0],"testmm",7) == 0 && j==1)
		startProcess(test_mm, 0, NULL, "testmm", foreground);

	else if(strncmp(tokens[0],"testproc",7) == 0)
		startProcess(test_proc, 0, NULL, "testproc", foreground);

	else if(strncmp(tokens[0], "pipe", 5) == 0 && j==1)
		startProcess(printPipeInfo, 0, NULL, "pipe", foreground);


	else if(strncmp(tokens[0],"testproc",7) == 0 && j==1)
		startProcess(test_proc, 0, NULL, "testproc", foreground);

	else if(strncmp(tokens[0],"testprio",7) == 0 && j==1)
		startProcess(test_prio, 0, NULL, "testprio", foreground);

	else if(strncmp(tokens[0],"testsync",7) == 0 && j==1)
		startProcess(test_sync, 0, NULL, "testsync", foreground);

	else if(strncmp(tokens[0],"testnosync",7) == 0 && j==1)
		startProcess(test_no_sync, 0, NULL, "testnosync", foreground);

	else
		printWarning();
	
}

    
/* --------------------------------------------------------------------------------------------------------------------------
                                        	OTHER METHODS
------------------------------------------------------------------------------------------------------------------------- */

void dummy(int argc, char **argv) {
    printf("Hola soy dummy", 0);
    while(1)
        halt();
}