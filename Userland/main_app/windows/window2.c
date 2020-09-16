/*---------------------------------------------------------------------------------------------------
|   WINDOW_2.C    |                                                                             	|
|----------------------                                                                             |
| This windows will display the shell program.														|
| Press ENTER to execute a command.																	|
| Press F1 to switch to calculator program.															|
---------------------------------------------------------------------------------------------------*/

#include <windows_lib.h>
#include <keyboard_lib.h>
#include <std_lib.h>
#include <asm_lib.h>
#include <syscalls.h>
#include <image_lib.h>

/* --------------------------------------------------------------------------------------------------------------------------
                                        		WINDOW DEFINITIONS
------------------------------------------------------------------------------------------------------------------------- */

static Window w;

#define W2_BUFFER_LEN 250

/* --------------------------------------------------------------------------------------------------------------------------
                                        		SHELL DEFINITIONS
------------------------------------------------------------------------------------------------------------------------- */

typedef enum
{
	NOCOMMAND,

	HELP,
	TIME,
	INFOREG,
	STOREDREG,
	PRINTMEM,
	CPUTEMP,
	CPUINFO,
	DIVZERO,
	INVOPCODE,
	CLEAR,
	DISPLAY_ANON,
	DISPLAY_MATRIX,
	WRONG
} command;

command parseCommand(char *buffer, int length, char *string);

static void help(void);

static void printTime(void);
static void printInfoReg(void);
static void printStoredReg(void);
static void printMemDump(char *start);
static void printCPUTemp(void);
static void printCPUInfo(void);
static void divZeroException(void);
static void invOpcodeException(void);
static void clearWindow(void);

static void printWarning(int num);
static int parseHexa(char *);

/* --------------------------------------------------------------------------------------------------------------------------
                                        	WINDOW METHODS
------------------------------------------------------------------------------------------------------------------------- */

/* -----------------------------------------------------------
 Defines the position and size of the window (all right half)
 and assings a color to title and body cursors
-------------------------------------------------------------- */

static void createWindow()
{

	ScreenRes res;
	getRes(&res);

	w.xi = res.width / 2 + 10;
	w.xf = res.width;
	w.yi = 0;
	w.yf = res.height;
	
	w.textBackground = 0;

	w.cursors[titleCursor].x = titleX;
	w.cursors[titleCursor].y = titleY;
	w.cursors[titleCursor].fontColor = titleColor;
	w.cursors[titleCursor].fontSize = titleSize;

	w.cursors[bodyCursor].x = 0;
	w.cursors[bodyCursor].y = bodyY;
	w.cursors[bodyCursor].fontColor = bodyColor;
	w.cursors[bodyCursor].fontSize = bodySize;
	w.cursors[bodyCursor].withIndicator = 1;

}

/* -----------------------------------------------------------
 Draws a line below the title to indicate that this windows
 is currently selected
-------------------------------------------------------------- */

static void drawIndicator(int color)
{

	for (int x = indicatorX; x < indicatorWidth; x++)
		drawPoint(x, indicatorY, indicatorHeight, color);
}

/* -----------------------------------------------------------
 Creates the window, and draws the title
-------------------------------------------------------------- */

void initWindow2()
{

	createWindow();
	setWindow(&w);

	w.activeCursor = titleCursor;
	printLine("Shell");
}

/* -------------------------------------------------------------
 Method that activates when this window becomes selected
 it waits for a key press constantly and handles it appropiately
---------------------------------------------------------------- */

void window2()
{

	setWindow(&w);
	drawIndicator(indicatorColor);

	//newLine();

	char bufferw2[W2_BUFFER_LEN + 1];
	cleanBuffer(bufferw2, W2_BUFFER_LEN);

	int bIter = 0;
	command currentCommand = WRONG;

	w.activeCursor = bodyCursor;

	while (1)
	{

		char c = getChar();

		// Switch to Window 1
		if (c == f1Code)
		{
			drawIndicator(0);
			drawCursor(0);
			return;
		}

		if(c == f3Code)
		{
			scrollUp(8);
		}

		if (c == '\b')
		{

			// Backspace pressed, backward the buffer

			if (bIter != 0)
			{
				bIter--;
				bufferw2[bIter] = 0;
			}
		}
		else if (isPrintableChar(c) && bIter < W2_BUFFER_LEN)
		{

			// Put the char into buffer

			bufferw2[bIter++] = c;
			if (bIter == W2_BUFFER_LEN)
				bIter++;
		}

		printChar(c);

		// Enter pressed, parse and execute command
		if (c == '\r')
		{
			if(bIter==0){
				newLine();
				continue;
			}

			// String for storing command parameter if present
			char parameter[W2_BUFFER_LEN];

			if (bIter > W2_BUFFER_LEN)
				currentCommand = WRONG;
			else
			{
				currentCommand = parseCommand(bufferw2, bIter, parameter);
			}

			switch (currentCommand)
			{
			case HELP:
				help();
				break;

			case TIME:
				printTime();
				break;

			case INFOREG:
				printInfoReg();
				break;

			case STOREDREG:
				printStoredReg();
				break;

			case PRINTMEM:
				printMemDump(parameter);
				break;

			case CPUTEMP:
				printCPUTemp();
				break;

			case CPUINFO:
				printCPUInfo();
				break;

			case DIVZERO:
				divZeroException();
				break;

			case INVOPCODE:
				invOpcodeException();
				break;

			case CLEAR:
				clearWindow();
				break;

			case DISPLAY_ANON:
				displayImage(ANONYMOUS, 20, 200);
				break;

			case DISPLAY_MATRIX:
				displayImage(MATRIX, 20, 200);
				break;

			case WRONG:
				printWarning(WRONG);
				break;

			default:
				printWarning(NOCOMMAND);
			}

			newLine();

			cleanBuffer(bufferw2, W2_BUFFER_LEN);
			bIter = 0;
			continue;
		}
	}
}

/* --------------------------------------------------------------------------------------------------------------------------
                                        SHELL METHODS
------------------------------------------------------------------------------------------------------------------------- */

/* -------------------------------------------------------------
						HELP
---------------------------------------------------------------- */

static void help(void)
{
	newLine();
	printLine("---------------------------------------------------");
	printLine("On this Terminal you can try the following commands:");
	
	newLine();

	printLine("- cputemp           to get the CPU Temp");
	printLine("- help              to go to the Help Manual");
	printLine("- printmem n        to print memory starting at n");
	printLine("- cpuinfo           to get the CPU Brand Data");
	printLine("- inforeg           to get the Register's Values");
	printLine("- storedreg         to get the Stored Register's Values");
	printLine("- time              to get the Time");
	printLine("- divzero           to execute a Div by Zero");
	printLine("- invopcode         to exectue an invalid opcode");
	printLine("- display anon      to show an image of Anonymous");
	printLine("- display matrix    to show an image of Matrix");
	printLine("- clear             to clear the screen");

	newLine();
	
	printLine("To go to calculator window, press F1");
	printLine("To scroll the window up, press F3");
	printLine("To store registers value, press TAB");
	
	newLine();
	
	printLine("---------------------------------------------------");
	newLine();
}

/* -------------------------------------------------------------
						TIME
---------------------------------------------------------------- */

static void printTime(void)
{
	Time t;
	getTime(&t);

	printf("\\nTime now: %2d:%2d:%2d\\n", 3, t.hours, t.minutes, t.seconds);
}

/* -------------------------------------------------------------
						INFOREG
---------------------------------------------------------------- */

static void printInfoReg(void)
{
	RegDump reg;
	regDump(&reg);

	printLine("Register's values:");
	printLine("--- --- --- --- --- --- --- --- --- --- --- --- ---");
	printf("\\n - rax - %x", 1, reg.rax);
	printf("\\n - rbx - %x", 1, reg.rbx);
	printf("\\n - rcx - %x", 1, reg.rcx);
	printf("\\n - rdx - %x", 1, reg.rdx);
	printf("\\n - rsi - %x", 1, reg.rsi);
	printf("\\n - rdi - %x", 1, reg.rdi);
	printf("\\n - rbp - %x", 1, reg.rbp);
	printf("\\n - rsp - %x", 1, reg.rsp);

	printf("\\n - r8 - %x", 1, reg.r8);
	printf("\\n - r9 - %x", 1, reg.r9);
	printf("\\n - r10 - %x", 1, reg.r10);
	printf("\\n - r11 - %x", 1, reg.r11);
	printf("\\n - r12 - %x", 1, reg.r12);
	printf("\\n - r13 - %x", 1, reg.r13);
	printf("\\n - r14 - %x", 1, reg.r14);
	printf("\\n - r15 - %x\\n", 1, reg.r15);

	printLine("--- --- --- --- --- --- --- --- --- --- --- --- ---");
}

/* -------------------------------------------------------------
						STOREDREG
---------------------------------------------------------------- */

static void printStoredReg(void)
{
	RegBkp reg;
	getRegBkp(&reg);

	printLine("Stored register's values:");
	printLine("--- --- --- --- --- --- --- --- --- --- --- --- ---");
	printf("\\n - rax - %x", 1, reg.rax);
	printf("\\n - rbx - %x", 1, reg.rbx);
	printf("\\n - rcx - %x", 1, reg.rcx);
	printf("\\n - rdx - %x", 1, reg.rdx);
	printf("\\n - rsi - %x", 1, reg.rsi);
	printf("\\n - rdi - %x", 1, reg.rdi);
	printf("\\n - rbp - %x", 1, reg.rbp);

	printf("\\n - r8 - %x", 1, reg.r8);
	printf("\\n - r9 - %x", 1, reg.r9);
	printf("\\n - r10 - %x", 1, reg.r10);
	printf("\\n - r11 - %x", 1, reg.r11);
	printf("\\n - r12 - %x", 1, reg.r12);
	printf("\\n - r13 - %x", 1, reg.r13);
	printf("\\n - r14 - %x", 1, reg.r14);
	printf("\\n - r15 - %x\\n", 1, reg.r15);

	printLine("--- --- --- --- --- --- --- --- --- --- --- --- ---");
}

/* -------------------------------------------------------------
						PRINTMEM
---------------------------------------------------------------- */

const int bufferMem = 33;

void printMemDump(char *sourceStr)
{

	uint64_t sourceHex = parseHexa(sourceStr);
	if (sourceHex < 0)
	{
		printLine("Parameter not allowed");
		return;
	}

	char *src = NULL;
	src = (char *)sourceHex;
	char *dst = src + 32;

	memDump((void *)src, (void *)dst);

	newLine();
	for (int i = 0; i < bufferMem; i += 8)
	{
		printf("%2x: %2x %2x %2x %2x %2x %2x %2x %2x\\n", 9, src + i, src[i], src[i + 1], src[i + 2], src[i + 3],
			   src[i + 4], src[i + 5], src[i + 6], src[i + 7]);
	}
	
}

/* -------------------------------------------------------------
						CPUTEMP
---------------------------------------------------------------- */

static void printCPUTemp(void)
{
	int temp = cpuTemp();
	printf("\\n Computer's Temperature: %d\\n", 1, temp);
}

/* -------------------------------------------------------------
						CPUINFO
---------------------------------------------------------------- */

static void printCPUInfo(void)
{
	CPUInfo info;

	char brandName[50], brandDesc[70];
	info.brandName = (char *)&brandName;
	info.brandDesc = (char *)&brandDesc;

	cpuInfo(&info);

	printf("\\nBrand name: %s", 1, info.brandName);
	printf("\\nBrand description: %s\\n", 1, info.brandDesc);
}

/* -------------------------------------------------------------
						DIVZERO
---------------------------------------------------------------- */

static void divZeroException(void)
{
	int a;
	a = 2 / 0;
}

/* -------------------------------------------------------------
						INVOPCODE
---------------------------------------------------------------- */

void invalidOpcode();

static void invOpcodeException(void)
{
	invalidOpcode();
}

/* -------------------------------------------------------------
						WARNING
---------------------------------------------------------------- */

static void printWarning(int num)
{
	printf("\\n >> Error: ", 0);
	switch (num)
	{
	case 0:
		printLine("Command not found");
		printLine("If you want to see the command manual type 'help'.");
		break;
	default:
		print("Something went wrong. ");
	}
	printf("Please, try again.\\n", 0);
}

static void clearWindow()
{
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

static int checkEmptySpace(char *buffer, int start, int length)
{

	for (int i = start; i <= length && buffer[i]; i++)
	{
		if (!isSpace(buffer[i]))
			return 0;
	}

	return 1;
}

static int isCommandHelp(char *buffer, int length)
{
	char *str = "help";
	if (!strncmp(str, buffer, 4))
		return 0;

	return checkEmptySpace(buffer, 4, length);
}

static int isCommandTime(char *buffer, int length)
{
	char *str = "time";
	if (!strncmp(str, buffer, 4))
		return 0;

	return checkEmptySpace(buffer, 4, length);
}

static int isCommandInfoReg(char *buffer, int length)
{
	char *str = "inforeg";
	if (!strncmp(str, buffer, 7))
		return 0;

	return checkEmptySpace(buffer, 7, length);
}

static int isCommandStoredReg(char *buffer, int length)
{
	char *str = "storedreg";
	if (!strncmp(str, buffer, 9))
		return 0;

	return checkEmptySpace(buffer, 9, length);
}

static int isCommandPrintmem(char *buffer, int length, char *start)
{
	if (!strncmp("printmem", buffer, 8))
		return 0;

	start[0] = '0';
	start[1] = 'x';
	if (checkEmptySpace(buffer, 8, length))
	{
		start[2] = '0';
		start[3] = 0;
		return 1;
	}

	char *aux2 = " 0x";
	if (!strncmp(aux2, buffer + 8, 3))
	{
		return 0;
	}

	int aux3 = 11;
	int i = 2;
	while (isHexa(*(buffer + aux3)) && aux3 < length && aux3 < (11 + 16))
	{
		start[i++] = buffer[aux3++];
	}
	start[i] = 0;

	return checkEmptySpace(buffer, aux3, length);
}

static int isCommandCPUTemp(char *buffer, int length)
{
	char *str = "cputemp";

	if (!strncmp(str, buffer, 7))
		return 0;

	return checkEmptySpace(buffer, 7, length);
}

static int isCommandCPUInfo(char *buffer, int length)
{
	char *str = "cpuinfo";
	if (!strncmp(str, buffer, 7))
		return 0;

	return checkEmptySpace(buffer, 7, length);
}

static int isCommandDivZero(char *buffer, int length)
{
	char *str = "divzero";
	if (!strncmp(str, buffer, 7))
		return 0;

	return checkEmptySpace(buffer, 7, length);
}


static int isCommandDisplayAnon(char *buffer, int length)
{
	char *str = "display anon";
	if (!strncmp(str, buffer, 12))
		return 0;

	return checkEmptySpace(buffer, 12, length);
}


static int isCommandDisplayMatrix(char *buffer, int length)
{
	char *str = "display matrix";
	if (!strncmp(str, buffer, 14))
		return 0;

	return checkEmptySpace(buffer, 14, length);
}

static int isCommandInvOpcode(char *buffer, int length)
{
	char *str = "invopcode";
	if (!strncmp(str, buffer, 9))
		return 0;

	return checkEmptySpace(buffer, 9, length);
}

static int isClearScreen(char *buffer, int length)
{
	char *str = "clear";
	if (!strncmp(str, buffer, 5))
		return 0;

	return checkEmptySpace(buffer, 5, length);
}

static int isAllowedChar(char c)
{
	if (isAlpha(c) || isDigit(c) || isSpace(c) || c == 0)
		return 1;
	return 0;
}

command parseCommand(char *buffer, int length, char *string)
{

	for (int i = 0; i < W2_BUFFER_LEN; i++)
	{
		if (!isAllowedChar(buffer[i]))
			return NOCOMMAND;
	}

	if (isCommandHelp(buffer, length) == 1)
		return HELP;

	if (isCommandTime(buffer, length))
		return TIME;

	if (isCommandInfoReg(buffer, length))
		return INFOREG;

	if (isCommandStoredReg(buffer, length))
		return STOREDREG;

	if (isCommandPrintmem(buffer, length, string))
		return PRINTMEM;

	if (isCommandCPUTemp(buffer, length))
		return CPUTEMP;

	if (isCommandCPUInfo(buffer, length))
		return CPUINFO;

	if (isCommandDivZero(buffer, length))
		return DIVZERO;

	if (isCommandDisplayAnon(buffer, length))
		return DISPLAY_ANON;

	if (isCommandDisplayMatrix(buffer, length))
		return DISPLAY_MATRIX;

	if (isCommandInvOpcode(buffer, length))
		return INVOPCODE;

	if (isClearScreen(buffer, length))
		return CLEAR;

	return NOCOMMAND;
}

static int parseHexa(char *start)
{
	int res = 0;
	if (start[0] != '0' || start[1] != 'x')
	{
		return -1;
	}

	for (int i = 2; start[i]; i++)
	{
		char c = '0';

		if (isDigit(start[i]))
			c = '0';
		else if (isLower(start[i]))
			c = 'a';
		else
			c = 'A';
		res = res * 16 + start[i] - c;
	}

	return res;
}
