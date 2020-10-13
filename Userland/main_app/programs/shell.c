/*---------------------------------------------------------------------------------------------------
|   SHELL.C    |                                                                             	    |
|----------------------                                                                             |
| This windows will display the shell program.														|
| Press ENTER to execute a command.																	|
---------------------------------------------------------------------------------------------------*/

#include <shell_builtins.h>
#include <syscalls.h>
#include <windows_lib.h>
#include <image_lib.h>
#include <std_lib.h>
#include <std_io.h>

/* --------------------------------------------------------------------------------------------------------------------------
                                        		SHELL DEFINITIONS
------------------------------------------------------------------------------------------------------------------------- */

#define MAX_TOKENS 10
#define MAX_TOKEN_LEN 20
#define MAX_CMD_LEN MAX_TOKENS*MAX_TOKEN_LEN

void parseCommand(char *cmdBuff);

void clearWindow(void);

void drawLine(int argc, char **argv);

extern void loop();

static Window w;


/* --------------------------------------------------------------------------------------------------------------------------
                                        	SHELL METHODS
------------------------------------------------------------------------------------------------------------------------- */

static void createWindow(){
	
	ScreenRes res;
	getRes(&res);

	w.xi = 0; w.xf = res.width;
	w.yi = 0; w.yf = res.height;
	
	w.textBackground = 0;

	w.cursors[bodyCursor].x = 0; w.cursors[bodyCursor].y = 0;
	w.cursors[bodyCursor].fontColor = bodyColor;
	w.cursors[bodyCursor].fontSize = bodySize;
	w.cursors[bodyCursor].withIndicator = 1;

}


void consumer(int argc, char **argv){

	static int x=0;

	block(getPid());

	for(; x<x+100; x++)
			draw(x, 100, 0xFF0000);

	// while(1){
	// 	waitSem(0);
	// 	for(; x<x+100; x++)
	// 		draw(x, 100, 0xFF0000);
	// }
}

void producer(int argc, char **argv){
	while(1){
		for(int i=0; i<9999; i++);
		postSem(0);
	}
}


void shell(){

	createWindow();
	setWindow(&w);

	char cmdBuff[MAX_CMD_LEN] = {0};
	int buffPos = 0;

	w.activeCursor = bodyCursor;

	if(createSem(0, 0) == -1){
		printf("Failed to create semaphore\n", 0);
	}

	// startProcess(consumer, 0, NULL, "consumer");
	// startProcess(producer, 0, NULL, "producer");


	char c;
	while ((c = getChar())){

		// Handle the chars that are not CR

		if(c == F3)
			scrollUp(8);

		if (c == '\b' && buffPos > 0)
			cmdBuff[--buffPos] = 0;
		
		if (isPrintableChar(c) && buffPos < MAX_CMD_LEN)
			cmdBuff[buffPos++] = c;

		printChar(c);

		// Handle the CR char, parse command

		if (c == '\r'){

			if(buffPos==0){
				newLine();
				continue;
			}

			cmdBuff[buffPos] = 0;
			parseCommand(cmdBuff);

			newLine();

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
		if(isSpace(cmdBuff[i])){
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
	
	else if(strncmp(tokens[0], "clear", 6) == 0)
		clearWindow();


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
	
	else if(strncmp(tokens[0], "heapstat", 9) == 0)
		printMemStatus();

	
	// Process management
	
	else if(strncmp(tokens[0], "ps", 3) == 0)
		printProcData();

	else if(strncmp(tokens[0], "kill", 5) == 0 && j==2)
		killProcess(tokens[1]);

	else if(strncmp(tokens[0], "block", 6) == 0 && j==2)
		blockProcess(tokens[1]);
	

	// New processes

	else if(strncmp(tokens[0], "line", 5) == 0)	
		startProcess(drawLine, 0, NULL, "line");

	else if(strncmp(tokens[0], "loop", 5) == 0)
		startProcess(loop, 0, NULL, "loop");


	// Command not found

	else
		printWarning();
	
}


/* --------------------------------------------------------------------------------------------------------------------------
                                        	OTHER METHODS
------------------------------------------------------------------------------------------------------------------------- */

void clearWindow(){
	for (int x = 0; x < w.xf; x++)
	{
		for (int y = 0; y < w.yf; y++)
		{
			//draw(x, y, 0);
			drawPoint(x, y, 1, 0);
		}
	}

	w.cursors[bodyCursor].x = 0;
	w.cursors[bodyCursor].y = bodyY;
}


void drawLine(int argc, char **argv){
	static int c=0;
	c+=1;

	for(int y=200*c; y<200*(c+1); y++)
		for(int x=0; x<1024; x++)
			for(int i=0; i<999; i++)
				draw(x,y,0xFF0000);

	kill(-1);	
}