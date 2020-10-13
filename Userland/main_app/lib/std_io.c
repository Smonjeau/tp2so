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

    while((c = read(STDIN)) == 0){
        sti();
        halt();
    }

    return ;

}

int putChar(char c){

    return write(c, STDOUT);

}