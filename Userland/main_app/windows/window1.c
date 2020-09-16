/*---------------------------------------------------------------------------------------------------
|   WINDOW_1.C    |                                                                             	|
|----------------------                                                                             |
| This windows will display the calculator program.													|
| Press ENTER to calculate an expression.															|
| Press ESC to drop the current expression.															|
| Press F2 to switch to shell program.																|
---------------------------------------------------------------------------------------------------*/

#include <windows_lib.h>
#include <keyboard_lib.h>
#include <std_lib.h>
#include <syscalls.h>
#include <stdint.h>
#include <image_lib.h>

/* --------------------------------------------------------------------------------------------------------------------------
                                        		WINDOW DEFINITIONS
------------------------------------------------------------------------------------------------------------------------- */

static Window w;

char *token;

#define W1_BUFFER_LEN 250

typedef enum
{
	CORRECT,
	BAD_EXPRESSION,
	DIVZERO,
	WRONG_CALC_CHAR,
	WRONG
} message;

message outputMsg;

/* --------------------------------------------------------------------------------------------------------------------------
                                        		CALCULATOR DEFINITIONS
------------------------------------------------------------------------------------------------------------------------- */

static void calculateString(char *s, int length);
static void printWarning(message num);
static int isAllowedChar(char c);
static int checkAllowedChars(char *s, int length);

static int checkCorrectMsg();
void match(char expected);
double exp();
double term();
double factor();
double readNumber();

/* -----------------------------------------------------------
 Defines the position and size of the window (all left half)
 and assings a color to title and body cursors
-------------------------------------------------------------- */

static void createWindow()
{

	ScreenRes res;
	getRes(&res);

	w.xi = 0;
	w.xf = res.width / 2 - 10;
	w.yi = 0;
	w.yf = res.height;

	w.cursors[titleCursor].x = titleX;
	w.cursors[titleCursor].y = titleY;
	w.cursors[titleCursor].fontColor = titleColor;
	w.cursors[titleCursor].fontSize = titleSize;

	w.textBackground = 0;

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
 Creates the window, draws the title, and a separator on the
 right end of the window.
-------------------------------------------------------------- */

void initWindow1()
{

	createWindow();
	setWindow(&w);

	w.activeCursor = titleCursor;
	printLine("Calculator");

	ScreenRes res;
	getRes(&res);

	for (int y = 0; y < w.yf; y++)
		drawPoint(res.width/2, y, 2, 0x00FF00);
}

/* -------------------------------------------------------------
 Method that activates when this window becomes selected
 it waits for a key press constantly and handles it appropiately
---------------------------------------------------------------- */

void window1()
{

	setWindow(&w);
	drawIndicator(indicatorColor);

	char bufferw1[W1_BUFFER_LEN + 1];
	cleanBuffer(bufferw1, W1_BUFFER_LEN);
	int bIter = 0;

	w.activeCursor = bodyCursor;

	while (1)
	{

		char c = getChar();

		if (c == f2Code)
		{
			drawCursor(0);
			drawIndicator(0);
			return;
		}

		if(c == f3Code)
		{
			scrollUp(8);
		}

		if (c == escCode)
		{
			clearLine();
			cleanBuffer(bufferw1, W1_BUFFER_LEN);
			bIter = 0;
		}
		else if (c == '\b')
		{
			if (bIter != 0)
			{
				bIter--;
				bufferw1[bIter] = 0;
			}
		}
		else if (isPrintableChar(c) && bIter < W1_BUFFER_LEN)
		{	
			bufferw1[bIter++] = c;
			if (bIter == W1_BUFFER_LEN)
				bIter++;
		}

		printChar(c);

		if (c == '=' || c == '\r')
		{
			newLine();
			if (bIter > W1_BUFFER_LEN)
			{
				printWarning(WRONG);
			}
			else
			{
				calculateString(bufferw1, bIter);
			}

			cleanBuffer(bufferw1, W1_BUFFER_LEN);
			bIter = 0;
		}
	}
}

/* --------------------------------------------------------------------------------------------------------------------------
                                        CALCULATOR METHODS
------------------------------------------------------------------------------------------------------------------------- */

void skipSpaces()
{
	while (isSpace(*token) && *token != 0)
	{
		token++;
	}
}

void calculateString(char *s, int length)
{
	if (!checkAllowedChars(s, length))
	{
		printWarning(WRONG_CALC_CHAR);
		return;
	}

	token = s;
	outputMsg = CORRECT;
	double result = exp();

	skipSpaces();
	if (*token != 0)
	{
		outputMsg = BAD_EXPRESSION;
	}

	if (outputMsg != CORRECT)
	{
		printWarning(outputMsg);
	}
	else
	{
		printf(" >> %f\\n", 1, result);
	}
}

static int checkAllowedChars(char *s, int length)
{
	for (int i = 0; i < length && s[i]; i++)
	{
		if (!isAllowedChar(s[i]))
			return 0;
	}
	return 1;
}

static int isAllowedChar(char c)
{

	if (isDigit(c) || isOperator(c) || isSpace(c) || isDecimalPoint(c) || c == 0)
		return 1;

	return 0;
}

/*Calculator inspired by recursive descent simple calculator implemented by
https://gist.github.com/mlabbe/81d667bac36aa60787fee60e3647a0a8
*/

static int checkCorrectMsg()
{
	if (outputMsg == CORRECT)
		return 1;
	return 0;
}

double exp()
{

	double value = term();
	if (!checkCorrectMsg())
		return 0;

	skipSpaces();
	while (*token == ADD || *token == SUBS)
	{
		if (*token == ADD)
		{
			match(ADD);
			if (!checkCorrectMsg())
				return 0;

			value += term();
			if (!checkCorrectMsg())
				return 0;
		}
		else if (*token == SUBS)
		{
			match(SUBS);
			if (!checkCorrectMsg())
				return 0;

			value -= term();
			if (!checkCorrectMsg())
				return 0;
		}
		else
		{
			outputMsg = BAD_EXPRESSION;
			return 0;
		}
		skipSpaces();
	}

	return value;
}

// match expected and move ahead
void match(char expected)
{
	if (*token == expected)
	{
		token++;
	}
	else
		outputMsg = BAD_EXPRESSION;
}

double term()
{
	double value = factor();

	skipSpaces();
	while (*token == MULT || *token == DIV)
	{

		if (*token == MULT)
		{

			match(MULT);
			if (!checkCorrectMsg())
				return 0;

			value *= factor();
			if (!checkCorrectMsg())
				return 0;
		}
		else if (*token == DIV)
		{

			match(DIV);
			if (!checkCorrectMsg())
				return 0;

			double aux = factor();

			if (!checkCorrectMsg())
			{
				return 0;
			}
			else if (aux < EPSILON && aux > -EPSILON)
			{
				outputMsg = DIVZERO;
				return 0;
			}
			value /= aux;
		}
		else
		{
			outputMsg = BAD_EXPRESSION;
			return 0;
		}
		skipSpaces();
	}
	return value;
}

double factor()
{
	double value = 0;

	skipSpaces();
	if (*token == PRTH_OP)
	{
		match(PRTH_OP);

		if (!checkCorrectMsg())
			return 0;

		value = exp();

		if (!checkCorrectMsg())
		{
			printLine("Ulala");
			return 0;
		}

		skipSpaces();
		match(PRTH_CL);

		if (!checkCorrectMsg())
			return 0;
	}
	else if (isDigit(*token) || isSign(*token))
	{
		// token--;

		value = readNumber();
		if (!checkCorrectMsg())
			return 0;
	}
	else
	{
		outputMsg = BAD_EXPRESSION;
		return 0;
	}
	return value;
}

double readNumber()
{
	double value = 0.0;
	int isNegative = 0;

	//Checks sign if it has one
	if (isSign(*token))
	{
		if (*token == SUBS)
			isNegative = 1;
		token++;
	}

	//Checks it doesn't begin with point
	if (isDecimalPoint(*token))
	{
		outputMsg = BAD_EXPRESSION;
		return 0;
	}

	while (isDigit(*token))
	{
		value = value * 10 + *token - '0';
		token++;
	}
	if (isDecimalPoint(*token))
	{
		token++;
		double aux = 0.1;
		while (isDigit(*token))
		{
			value += (*token - '0') * aux;
			aux /= 10;
			token++;
		}
	}

	//Makes negative if flag activated
	if (isNegative)
		value = (-1) * value;
	outputMsg = CORRECT;

	return value;
}

/* -------------------------------------------------------------
						WARNING
---------------------------------------------------------------- */

static void printWarning(message msg)
{
	printf("\\n >> Error: ", 0);

	switch (msg)
	{
	case DIVZERO:
		print("Zero division is not allowed.");
		break;
	case WRONG_CALC_CHAR:
		print("To calculate use only numbers or the following operators: ");
		printLine("+ - x % ( ) , .");
		break;
	case BAD_EXPRESSION:
		printLine("Bad expression.");
		char msg[2] = "?";
		msg[0] = (char)token[0];
		printf("Unexpected '%s' in input.\\n", 1, msg);
		break;
	case WRONG:
		print("Something went wrong.");
		break;
	default:
		printf("Got a completely unexpected error code: %d", 1, msg);
	}

	printLine(" Please, try again.");
}