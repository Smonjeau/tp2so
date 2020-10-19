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

void parseCommand(char *cmdBuff) {

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

	int foreground = 1;
	if(strncmp(tokens[j-1], "&", 2) == 0){
		foreground = 0;
		j -= 1;
	}

	// Miscellaneous

	if (strncmp(tokens[0], "help", 5) == 0 && j==1)
		printHelp();

	else if (strncmp(tokens[0], "divzero", 8) == 0 && j==1)
		divZeroException();

	else if (strncmp(tokens[0], "invopcode", 10) == 0 && j==1)
		invOpcodeException();

	else if (strncmp(tokens[0], "time", 5) == 0 && j==1)
		printTime();

	else if(strncmp(tokens[0], "display", 8) == 0 && j==2)
		displayImage(tokens[1], 20, 200);


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
		printCPUTemp();

	else if (strncmp(tokens[0], "cpuinfo", 8) == 0 && j==1)
		printCPUInfo();

	else if (strncmp(tokens[0], "inforeg", 8) == 0 && j==1)
		printInfoReg();

	else if (strncmp(tokens[0], "storedreg", 10) == 0 && j==1)
		printStoredReg();


	// Memory management

	else if (strncmp(tokens[0], "memdump", 8) == 0 && j == 2)
		printMemDump(tokens[1]);

	else if (strncmp(tokens[0], "mem", 4) == 0 && j==1)
		printMemStatus();


	// Process management

	else if (strncmp(tokens[0], "kill", 5) == 0 && j == 2)
		killProcess(tokens[1]);

	else if (strncmp(tokens[0], "block", 6) == 0 && j == 2)
		blockProcess(tokens[1]);

	else if (strncmp(tokens[0], "nice", 5) == 0 && j == 3)
		niceProcess(tokens[1], tokens[2]);

	else if(strncmp(tokens[0],"ps",3)==0 && j==1)
		printProcData();


	// New processes

	else if(strncmp(tokens[0], "line", 5) == 0 && j==1)	
		startProcess(line, 0, NULL, "line", foreground);

	else if (strncmp(tokens[0], "loop", 5) == 0 && j==1)
		startProcess(loop, 0, NULL, "loop", foreground);


	//Sync

	else if (strncmp(*tokens, "sem", 4) == 0 && j==1)
		printSemStatus();

	else if(strncmp(tokens[0], "pipe", 5) == 0 && j==1)
		printPipeInfo();


	//Testing

	else if(strncmp(tokens[0],"testmm",7) == 0 && j==1)
		startProcess(test_mm, 0, NULL, "testmm", foreground);

	else if(strncmp(tokens[0],"testproc",7) == 0)
		startProcess(test_proc, 0, NULL, "test_proc", foreground);

	else if(strncmp(tokens[0], "pipe", 5) == 0 && j==1)
		printPipeInfo();


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