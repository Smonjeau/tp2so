/*---------------------------------------------------------------------------------------------------
|   _LOADER.C    |	                                                                                |
|-----------------    		                                                                        |
| These functions handles messages received from the kernel, forwarding them to appropiate handlers.|
| It supports messages for starting the main app, and being notified of an exception or IRQ.        |
---------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <kernel_messages.h>
#include <programs.h>
#include <syscalls.h>

extern char bss;
extern char endOfBinary;


int _start(int message, int nargs, ...) {

	va_list valist;
    va_start(valist, nargs);

	switch(message){
		case START_SHELL:
			return shell();
			//startProcess(shell, 0, (void*) 0);
			//__asm__("hlt;\n\r");

		case EXCEPTION_PRODUCED:
			return exception(valist);

		case IRQ_PRODUCED:
			return interruption(valist);

		default:
			return shell();
	}

}