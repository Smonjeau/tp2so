#include <stdarg.h>
#include <stdint.h>
#include <video_lib.h>
#include <lib.h>

/* ----------------------------------------------------------------------------------------------------------------------
                                                    MEMORY METHODS
---------------------------------------------------------------------------------------------------------------------- */

void * memset(void * destination, int32_t c, uint64_t length){
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}


void * memcpy(void * destination, const void * source, uint64_t length){
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


/* ----------------------------------------------------------------------------------------------------------------------
                                                    SYNCH METHODS
---------------------------------------------------------------------------------------------------------------------- */

void acquire(int *lock){
    while(_xchg(lock, 1) != 0);
}


void release(int *lock){
    _xchg(lock, 0);
}


/* ----------------------------------------------------------------------------------------------------------------------
                                                    STRING METHODS
---------------------------------------------------------------------------------------------------------------------- */

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

void printf(char *format, int nargs, ...){

    va_list valist;

    va_start(valist, nargs);

    int pos, formatChar = 0, fixLen = -1;
    for (pos = 0; format[pos] != 0; pos++){

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
                char str[20]={0};
                print(itoa(va_arg(valist, int), str, 10, fixLen));
                formatChar = 0;
                continue;
            }

            if (format[pos] == 'x'){
                char str[20]={0};
                print("0x");
                print(itoa(va_arg(valist, int), str, 16, fixLen));
                formatChar = 0;
                continue;
            }

            if (format[pos] == 'f'){
                char str[20]={0};
                print(dtoa(va_arg(valist, double), str));
                formatChar = 0;
                continue;
            }

            if (format[pos] == 's'){
                
                print(va_arg(valist, char *));
                
                formatChar = 0;
                continue;
            }
        }

        printChar(format[pos]);

    }

    va_end(valist);
}


void strcpy(char *src, char *dest){
    int i;
    for (i = 0; src[i] != 0; i++)
        dest[i] = src[i];
    dest[i] = 0;
}


int strlen(char * source) {
	int count = 0;
	while(source[count] != 0)
		count++;
	return count;
}


void strcat(const char* source, char* destination){
	// make ptr point to the end of destination string
	char* ptr = destination + strlen(destination);

	// Appends characters of source to the destination string
	while (*source != '\0')
		*ptr++ = *source++;

	// null terminate destination string
	*ptr = '\0';
}