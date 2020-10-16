#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
char *cpuVendor(char *result);

int div_ceil(int dividend, int divisor);

// SYNCHRONIZATION METHODS

void acquire(int *lock);

void release(int *lock);

int _xchg();

void strcpy(char *src, char *dest);

void strcat(const char * source, char * destination);

int strlen(char * source);

char *itoa(int num, char *str, int base, int fixLen);

#endif