/*---------------------------------------------------------------------------------------------------
|   STDLIB.C    |                                                                                      |
|----------------                                                                            |
| This libary implements a bare version of C stdlib, providing functions such as prinf, itoa, etc.  |
---------------------------------------------------------------------------------------------------*/

#include <keyboard_lib.h>
#include <windows_lib.h>
#include <stdarg.h>
#include <std_lib.h>
#include <stdint.h>


/* ------------------------------------------------------------------------------------------------------------------
                                                MEMORY FUNCTIONS
 -------------------------------------------------------------------------------------------------------------------- */

void * memcpy(void * destination, const void * source, int length){
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}


/* ------------------------------------------------------------------------------------------------------------------
                                                CHAR FUNCTIONS
 -------------------------------------------------------------------------------------------------------------------- */

int isLower(char c)
{
    if (c >= 'a' && c <= 'z')
        return 1;
    return 0;
}

int isUpper(char c)
{
    if (c >= 'A' && c <= 'Z')
        return 1;
    return 0;
}

char toLower(char c)
{
    if (isUpper(c))
        return c + 'a' - 'A';
    return c;
}

int isAlpha(char c)
{
    if (isLower(c) || isUpper(c))
        return 1;
    return 0;
}

int isDigit(char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    return 0;
}

int isHexa(char c)
{
    if (isDigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
        return 1;
    return 0;
}

int isDecimalPoint(char c)
{
    if (c == '.' || c == ',')
        return 1;
    return 0;
}

int isSpace(char c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == 0)
        return 1;
    return 0;
}

int isOperator(char c)
{
    if (isSign(c) || c == MULT || c == DIV || isParenthesis(c) != 0)
        return 1;
    return 0;
}

int isSign(char c)
{
    if (c == ADD || c == SUBS)
        return 1;
    return 0;
}

int isParenthesis(char c)
{
    if (c == PRTH_OP)
        return 1;
    if (c == PRTH_CL)
        return -1;

    return 0;
}

// The extended ASCII is not supported by the used font, thus they are marked as not printable

int isPrintableChar(char c){
    return c >= 32 && c <= 127;
}

/* ------------------------------------------------------------------------------------------------------------------
                                            STRING FUNCTIONS
 -------------------------------------------------------------------------------------------------------------------- */

/* -----------------------------------------------------------
 Function used in itoa and dtoa
 Taken from https://www.geeksforgeeks.org/implement-itoa/
----------------------------------------------------------- */

void reverseStr(char *str, int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        char aux = str[start];
        str[start] = str[end];
        str[end] = aux;
        start++;
        end--;
    }
}

/* -----------------------------------------------------------
 Function to convert an integer to a string
----------------------------------------------------------- */

char *itoa(int num, char *str, int base, int fixLen)
{
    int i = 0;
    int isNegative = 0;

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
    }

    if (num < 0)
    {
        isNegative = -1;
        num = -num;
    }

    while (num != 0 && (i < fixLen || fixLen < 0))
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        num = num / base;
    }

    if (isNegative == 1 && base == 10)
        str[i++] = '-';

    while (i < fixLen)
    {
        str[i++] = '0';
    }

    str[i] = '\0';

    reverseStr(str, i);

    return str;
}

/* -----------------------------------------------------------
 Function to convert a double to a string
----------------------------------------------------------- */

char *dtoa(double num, char *str)
{

    int isNegative = 0;
    if (num < 0)
    {
        isNegative = 1;
        num = -num;
    }

    int i = 0;

    if (num < EPSILON && num > -EPSILON)
    {
        str[i++] = '0';
        str[i++] = '.';
        str[i++] = '0';

        str[i] = '\0';
        return str;
    }

    int auxNum = (int)num;
    // --- Building the int part ---
    while (auxNum > EPSILON)
    {
        int rem = auxNum % 10;
        str[i++] = rem + '0';
        auxNum = auxNum / 10;
    }

    if (isNegative == 1)
        str[i++] = '-';

    str[i] = '\0';
    str[i] = '\0';
    reverseStr(str, i);
    str[i++] = '.';

    for (int j = 0; j < DTOA_FLOAT_MAX_LEN; j++)
    {
        num = num * 10;
        int aux = (int)num;
        str[i++] = aux % 10 + '0';
    }
    return str;
}

/* -----------------------------------------------------------
 Printf that supports integers, doubles, strings and hex
 display as possible formats
-------------------------------------------------------------- */

void printf(char *format, int nargs, ...)
{

    va_list valist;

    va_start(valist, nargs);

    int pos, formatChar = 0, specialChar = 0, fixLen = -1;
    for (pos = 0; format[pos] != 0; pos++)
    {

        if (format[pos] == '%')
        {
            formatChar = 1;
            continue;
        }

        if (formatChar == 1)
        {

            if (isDigit(format[pos]))
            {
                fixLen = format[pos] - '0';
                continue;
            }

            if (format[pos] == 'd')
            {
                char str[20];
                print(itoa(va_arg(valist, int), str, 10, fixLen));
                formatChar = 0;
                continue;
            }

            if (format[pos] == 'x')
            {
                char str[20];
                print("0x");
                print(itoa(va_arg(valist, int), str, 16, fixLen));
                formatChar = 0;
                continue;
            }

            if (format[pos] == 'f')
            {
                char str[20];
                print(dtoa(va_arg(valist, double), str));
                formatChar = 0;
                continue;
            }

            if (format[pos] == 's')
            {
                print(va_arg(valist, char *));
                formatChar = 0;
                continue;
            }
        }

        if (format[pos] == '\\')
        {
            specialChar = 1;
            continue;
        }

        if (specialChar == 1)
        {

            specialChar = 0;

            if (format[pos] == 'n')
            {
                printLine("");
                continue;
            }
        }

        printChar(format[pos]);
    }

    va_end(valist);
}

/* -----------------------------------------------------------
 Compares two strings until one of the two ends and returns 1
 if they are equal or 0 otherwise
----------------------------------------------------------- */

int strcmp(char *str1, char *str2)
{

    for (int i = 0; str1[i] != 0 || str2[i] != 0; i++)
    {
        if (str1[i] != str2[i])
            return 0;
    }

    return 1;
}

/* --------------------------------------------------------------
 Compares a certain amount of chars of two strings and returns 1
 if they are equal or 0 otherwise
-------------------------------------------------------------- */

int strncmp(char *str1, char *str2, int length)
{
    int i;

    for (i = 0; i < length && str1[i] && str2[i]; i++)
    {
        if (str1[i] != str2[i])
            return 0;
    }

    if (str1[i] != 0 && str2[i])
        return 0;

    return 1;
}

void strcpy(char *src, char *dest)
{
    int i;
    for (i = 0; src[i] != 0; i++)
        dest[i] = src[i];
    dest[i] = 0;
}
