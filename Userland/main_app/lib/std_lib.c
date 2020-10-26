/*---------------------------------------------------------------------------------------------------
|   STDLIB.C    |                                                                                      |
|----------------                                                                            |
| This libary implements a bare version of C stdlib, providing functions such as prinf, itoa, etc.  |
---------------------------------------------------------------------------------------------------*/

#include <stdarg.h>
#include <stdint.h>
#include <std_io.h>
#include <std_lib.h>
#include <syscalls.h>
#define NULL (void*)0

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


void * memset(void * destination, int32_t c, uint64_t length){
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}


/* ------------------------------------------------------------------------------------------------------------------
                                            STRING FUNCTIONS
 -------------------------------------------------------------------------------------------------------------------- */

/* -----------------------------------------------------------
 Function used in itoa and dtoa
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
 Function used in atoi and printf
----------------------------------------------------------- */

int isDigit(char c){
    if (c >= '0' && c <= '9')
        return 1;
    return 0;
}


/* -----------------------------------------------------------
 Function to convert an string to integer.
----------------------------------------------------------- */

int atoi(char* str){ 
    if (*str == '\0') 
        return 0; 
  
    int res = 0; 
    int sign = 1; 
    int i = 0; 
  
    if (str[0] == '-') { 
        sign = -1; 
        i++; 
    } 
  
    for (; str[i] != '\0'; ++i) { 
        if (!isDigit(str[i])) 
            return -1;   
        res = res * 10 + str[i] - '0'; 
    } 
  
    return sign * res; 
} 


/* -----------------------------------------------------------
 Function to convert an integer to a string
----------------------------------------------------------- */

int  itoa(int num, char *str, int base, int fixLen)
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
        isNegative = 1;
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

    return i;
}


/* -----------------------------------------------------------
 Function to convert a double to a string
----------------------------------------------------------- */

int dtoa(double num, char *str)
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
        return i;
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
    return i;
}


/* -----------------------------------------------------------
 Printf that supports integers, doubles, strings and hex
 display as possible formats
-------------------------------------------------------------- */

void printf(char *format, int nargs, ...){

    va_list valist;

    va_start(valist, nargs);

    int pos, formatChar = 0, fixLen = -1;
    int buffPos=0;
    char buffer[255]={0};
    for (pos = 0; format[pos] != 0; pos++){

  
        if(format[pos]=='\n'){
            //Imprimo y flusheo buffer
            buffer[buffPos]=0;
            write(1,buffer,buffPos+1);
            putChar('\n');
            buffPos=0;
            buffer[0] = 0;
            continue;
        }
        if (format[pos] == '%'){
            formatChar = 1;
            continue;
        }

        if (formatChar == 1){

            if (isDigit(format[pos])){
                fixLen = format[pos] - '0';
                continue;
            }

            if (format[pos] == 'd'){
                int len = itoa(va_arg(valist,int),buffer+buffPos,10,fixLen);
           //     for(int i=0;i<len;i++)
             //     buffer[buffPos++]=str[i];
                buffPos +=len;
                
                formatChar = 0;
                continue;
            }

            if (format[pos] == 'x'){
                buffer[buffPos++]='0';
                buffer[buffPos++]='x';
                int len =itoa(va_arg(valist, int), buffer + buffPos, 16, fixLen);
                buffPos += len;
                formatChar = 0;
                continue;
            }

            if (format[pos] == 'f'){
                int len = dtoa(va_arg(valist, double), buffer + buffPos);
                len += buffPos;
                formatChar = 0;
                continue;
            }

            if (format[pos] == 's'){
                

                int len = strcpy(va_arg(valist, char *),buffer + buffPos);
                buffPos += len;
                formatChar = 0;
                continue;
            }
        }
        else if(format[pos]!=0)
            buffer[buffPos++] = format[pos];

    }
        write(1,buffer,buffPos+1);

    va_end(valist);
}


/* --------------------------------------------------------------
 Compares a certain amount of chars of two strings and returns 1
 if they are equal or 0 otherwise
-------------------------------------------------------------- */

int strncmp(char *s1, char *s2, int n){
    unsigned char u1, u2;

    while (n-- > 0){
        u1 = (unsigned char) *s1++;
        u2 = (unsigned char) *s2++;
        if (u1 != u2)
            return u1 - u2;
        if (u1 == '\0')
            return 0;
    }

    return 0;
}


/* --------------------------------------------------------------
 Copies a string from src to dest
-------------------------------------------------------------- */

int strcpy(char *src, char *dest){
    int i;
    for (i = 0; src[i] != 0; i++)
        dest[i] = src[i];
    dest[i] = 0;
    return i;
}

void nap(uint64_t limit){
    for (int i =0;i<limit;i++);
}