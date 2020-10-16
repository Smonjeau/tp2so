#include <stdint.h>
#include <lib.h>

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
int div_ceil(int dividend, int divisor){
    if (dividend%divisor == 0)
        return  dividend / divisor;
    else
        return (dividend / divisor) +1;
}

void acquire(int *lock){
    while(_xchg(lock, 1) != 0);
}

void release(int *lock){
    _xchg(lock, 0);
}

void strcpy(char *src, char *dest)
{
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

void strcat(const char* source, char* destination)
{
	// make ptr point to the end of destination string
	char* ptr = destination + strlen(destination);

	// Appends characters of source to the destination string
	while (*source != '\0')
		*ptr++ = *source++;

	// null terminate destination string
	*ptr = '\0';

}

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

