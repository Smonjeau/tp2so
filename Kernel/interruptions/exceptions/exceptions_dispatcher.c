/*---------------------------------------------------------------------------------------------------
|   EXCEPTIONS_DISPATCHER.C    |                                                                    |
|-------------------------------                                                                    |
| This function attends exceptions and shows a message with information								|
---------------------------------------------------------------------------------------------------*/

#include <video_lib.h>
#include <lib.h>

typedef struct RegBkp{
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
} RegBkp;

void dumpState(RegBkp *dest);



int handleException(int id, void *eip);


void exceptionDispatcher(int id, void * eip) {

	handleException(id, eip);

}


int handleException(int id, void *eip){

	printf("\f", 0);
    printf("Exception code: %d\n", 1, id);

    RegBkp bkp;
    dumpState(&bkp);

    printf("\nFeed this information to the developers:", 0);
    printf("\nRAX: %x - RBX: %x - RCX: %x - RDX: %x", 4, bkp.rax, bkp.rbx, bkp.rcx, bkp.rcx);
    printf("\nRIP: %x - RBP: %x - RSI: %x - RDI: %x", 4, eip, bkp.rbp, bkp.rsi, bkp.rdi);
    printf("\n\nPress enter to restart the system", 0);

    while(1);

    return 1;
    
}