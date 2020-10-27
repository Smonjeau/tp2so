// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*---------------------------------------------------------------------------------------------------
|   std_io.C    |                                                                                |
|----------------------                                                                             |
| This library has methods for managing the standar input and output                                |
---------------------------------------------------------------------------------------------------*/

#include <syscalls.h>
#include <asm_lib.h>
#include <std_io.h>

char getChar(){

	char c;

	read(STDIN, &c, 1);

	return c;

}

int putChar(char c){
    
    return write(STDOUT, &c, 1);

}