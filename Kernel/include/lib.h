#ifndef LIB_H
#define LIB_H

#include <stdint.h>

// MEMORY METHODS

void * memset(void * destination, int32_t c, uint64_t length);

void * memcpy(void * destination, const void * source, uint64_t length);


// SYNCHRONIZATION METHODS

void acquire(int *lock);

void release(int *lock);

int _xchg();


// STRING METHODS

#define DTOA_FLOAT_MAX_LEN 4
#define EPSILON 0.0001

int atoi (char * str);

char *itoa(int num, char *str, int base, int fixLen);

char *dtoa(double num, char *str);

void printf(char *format, int nargs, ...);

void strcpy(char *src, char *dest);

void strcat(const char * source, char * destination);

int strlen(char * source);


#endif