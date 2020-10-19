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

<<<<<<< HEAD
=======
void testPrint2(){

	printf("PRINTED\n", 0);

	kill(-1);

}

void testPrint(){

	startProcess(testPrint2, 0, NULL, "test_sync", 1);

	kill(-1);

}


>>>>>>> 947f4230bd1e7f8390d7c0188b8bb341020ca4a7
void shell(){

    startProcess(dummy, 0, (void *) 0, "dummy", 1); //Necesario en ciertos casos

	forcePipe(0); //Creamos el pipe que comunica fd 0 con teclado
	forcePipe(1); //Creamos el pipe que comunica fd 1 con salida estandar

	char cmdBuff[MAX_CMD_LEN] = {0};
	int buffPos = 0;

	printf("\f", 0);

	startProcess(test_sync, 0, NULL, "test_no_sync");

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

	else if(strncmp(tokens[0], "filter", 7) == 0)
		startFilter();
	
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
			startProcess(line, 0, NULL, "line", 1);

        else if (strncmp(tokens[0], "loop", 5) == 0 && j==1)
            startProcess(loop, 0, NULL, "loop", 1);


        //Sync

        else if (strncmp(*tokens, "sem", 4) == 0 && j==1)
            printSemStatus();

		else if(strncmp(tokens[0], "pipe", 5) == 0)
			printPipeInfo();


		//Testing

		else if(strncmp(tokens[0],"testmm",7) == 0 && j==1)
			startProcess(test_mm, 0, NULL, "testmm", 1);

		else if(strncmp(tokens[0],"testproc",7) == 0)
			startProcess(test_proc,0,NULL,"test_proc", 1);

		else if(strncmp(tokens[0], "pipe", 5) == 0 && j==1)
			printPipeInfo();

		else if(strncmp(tokens[0],"filter",7) == 0 && j==1)
			startFilter();

		else if(strncmp(tokens[0],"cat",4)==0 && j==1){
			// int fds [2];
			// pipe(fds);
			// //Solo escribe
			// close(fd[0]);
			// startProcess(cat,0,NULL,"cat");
		}

		else if(strncmp(tokens[0],"testproc",7) == 0 && j==1)
			startProcess(test_proc, 0, NULL, "testproc", 1);

		else if(strncmp(tokens[0],"testsync",7) == 0 && j==1)
			startProcess(test_sync, 0, NULL, "testsync", 1);

		else if(strncmp(tokens[0],"testnosync",7) == 0 && j==1)
			startProcess(test_no_sync, 0, NULL, "testnosync", 1);

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