/*---------------------------------------------------------------------------------------------------
|   WINDOW_2.C    |                                                                             	|
|----------------------                                                                             |
| This windows will display the shell program.														|
| Press ENTER to execute a command.																	|
| Press F1 to switch to calculator program.															|
---------------------------------------------------------------------------------------------------*/

#include <shell_builtins.h>
#include <syscalls.h>
#include <windows_lib.h>
#include <image_lib.h>
#include <keyboard_lib.h>
#include <std_lib.h>

/* --------------------------------------------------------------------------------------------------------------------------
                                        		WINDOW DEFINITIONS
------------------------------------------------------------------------------------------------------------------------- */

static Window w;


/* --------------------------------------------------------------------------------------------------------------------------
                                        		SHELL DEFINITIONS
------------------------------------------------------------------------------------------------------------------------- */

#define MAX_TOKENS 10
#define MAX_TOKEN_LEN 20
#define MAX_CMD_LEN MAX_TOKENS*MAX_TOKEN_LEN

void parseCommand(char *cmdBuff);

static void clearWindow(void);

static int parseHexa(char *);

extern void loop();


/* --------------------------------------------------------------------------------------------------------------------------
                                        	WINDOW METHODS
------------------------------------------------------------------------------------------------------------------------- */

/* -----------------------------------------------------------
 Defines the position and size of the window (all right half)
 and assings a color to title and body cursors
-------------------------------------------------------------- */
static void drawLine(int argc, char **argv){
	printf("Hola soy el proceso q dibuja una linea, mi pid es %d \\n",1,getPid());
	//int i;
	//for(int i=0;i< 0xffffffff;i++);
	static int x3=0;
	static int y3 = 400;
	int cant=0;
	while(cant<4) {
		x3=0;
		for(; x3<1024; x3++)
			draw(x3, y3, 0x0000FF);
		y3 += 6;
		cant++;

	}
	//kill(-1);	
}

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

/* -------------------------------------------------------------
 Method that activates when this window becomes selected
 it waits for a key press constantly and handles it appropiately
---------------------------------------------------------------- */

void shell(){

	createWindow();
	setWindow(&w);

	char cmdBuff[MAX_CMD_LEN] = {0};
	int buffPos = 0;

	w.activeCursor = bodyCursor;

	char c;
	while ((c = getChar())){

		// Handle the chars that are not CR

		if(c == f3Code)
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
                                        SHELL METHODS
------------------------------------------------------------------------------------------------------------------------- */

static void clearWindow(){
	for (int x = 0; x < w.xf; x++)
	{
		for (int y = bodyY; y < w.yf; y++)
		{
			//draw(x, y, 0);
			drawPoint(x, y, 1, 0);
		}
	}

	w.cursors[bodyCursor].x = 0;
	w.cursors[bodyCursor].y = bodyY;
}

/* --------------------------------------------------------------------------------------------------------------------------
                                COMMAND-CHECK METHODS
------------------------------------------------------------------------------------------------------------------------- */

void parseCommand(char *cmdBuff){

	// Separate on tokens

	char *tokens[MAX_TOKENS] = {0};
	tokens[0] = cmdBuff;

	for (int i=0, j=1; cmdBuff[i]; i++){
		if(isSpace(cmdBuff[i])){
			cmdBuff[i] = 0;
			tokens[j++] = cmdBuff+i+1;
		}
	}

	// Miscellaneous

	if(strncmp(tokens[0], "help", 5))
		printHelp();

	else if(strncmp(tokens[0], "divzero", 8))
		divZeroException();

	else if(strncmp(tokens[0], "invopcode", 10))
		invOpcodeException();

	else if(strncmp(tokens[0], "time", 5))
		printTime();

	else if(strncmp(tokens[0], "display", 8))
		displayImage(tokens[1], 20, 200);
	
	else if(strncmp(tokens[0], "clear", 6))
		clearWindow();


	// CPU management

	else if(strncmp(tokens[0], "cputemp", 8))
		printCPUTemp();

	else if(strncmp(tokens[0], "cpuinfo", 8))
		printCPUInfo();

	else if(strncmp(tokens[0], "inforeg", 8))
		printInfoReg();

	else if(strncmp(tokens[0], "storedreg", 10))
		printStoredReg();
	

	// Memory management

	else if(strncmp(tokens[0], "printmem", 9))
		printMemDump(tokens[1]);
	
	else if(strncmp(tokens[0], "mem", 4))
		printMemStatus();

	
	// Process management
	
	else if(strncmp(tokens[0], "ps", 3))
		printProcData();

	else if(strncmp(tokens[0], "kill", 5))
		killCommand(tokens[1]);

	else if(strncmp(tokens[0], "block", 6))
		blockProcess(tokens[1]);
	

	// New processes

	else if(strncmp(tokens[0], "line", 5))	
		startProcess(drawLine, 0, NULL, "line");

	else if(strncmp(tokens[0], "loop", 5))
		startProcess(loop, 0, NULL, "loop");


	// Command not found

	else
		printWarning();
	
}