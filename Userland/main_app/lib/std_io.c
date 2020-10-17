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
	read(0, &c, 1);
	return c;
    /*
	//WTF ES ESTO BRO
    char c;

    while((c = read(STDIN)) == 0){
        sti();
        halt();
    }

    return c;*/

}

int putChar(char c){
	return 0;
    //return write(c, STDOUT);

}