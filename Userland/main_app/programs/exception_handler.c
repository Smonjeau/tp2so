/*---------------------------------------------------------------------------------------------------
|   EXCEPTION_HANDLER.C    |                                                                        |
|---------------------------                                                                        |
| This windows display exception information when they are produced. This includes common registers |
| at the moment of the exception, and which was the exception produced.		    			        |
---------------------------------------------------------------------------------------------------*/

#include <programs.h>
#include <std_lib.h>
#include <std_io.h>
#include <syscalls.h>
#include <kernel_messages.h>

/* --------------------------------------------------------------------------------------------------------------------------
                                        		DEFINITIONS
------------------------------------------------------------------------------------------------------------------------- */

#define cursor 0


/* --------------------------------------------------------------------------------------------------------------------------
                                        		METHODS
------------------------------------------------------------------------------------------------------------------------- */

int exception(va_list args){

    int id = va_arg(args, int);
    void * eip = va_arg(args, void *);

    putChar('\f');

    switch(id){
        case DIV_ZERO_EXCEPTION:
            printf("Division by zero detected!\n", 0);
            printf("Possible causes: \n", 0);
            printf(" - You didn't finish elementary school\n", 0);
            break;

        case INV_OPCODE_EXCEPTION:
            printf("Invalid opcode detected!\n", 0);
            printf("Possible causes: \n", 0);
            printf(" - You received an EMP attack\n", 0);
            printf(" - You tried to destroy the machine\n", 0);
            break;

        default:
            printf("Exception code: %d\n", 1, id);
    }

    RegBkp bkp;
    getRegBkp(&bkp);

    printf("\nFeed this information to the developers:", 0);
    printf("\nRAX: %x - RBX: %x - RCX: %x - RDX: %x", 4, bkp.rax, bkp.rbx, bkp.rcx, bkp.rcx);
    printf("\nRIP: %x - RBP: %x - RSI: %x - RDI: %x", 4, eip, bkp.rbp, bkp.rsi, bkp.rdi);
    printf("\n\nPress enter to restart the system", 0);

    while(1);

    return 1;
    
}