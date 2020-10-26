#ifndef STD_LIB_H
#define STD_LIB_H

#include <inttypes.h>

// MEMORY METHODS

void * memcpy(void * destination, const void * source, int length);
void * memset(void * destination, int32_t c, uint64_t length);

// STRING METHODS

#define DTOA_FLOAT_MAX_LEN 4
#define EPSILON 0.0001

int atoi (char * str);

int itoa(int num, char *str, int base, int fixLen);

int dtoa(double num, char *str);

void printf(char *format, int nargs, ...);

int strncmp(char *s1, char *s2, int length);

int strcpy(char *src, char *dest);

void nap (uint64_t limit);

#endif