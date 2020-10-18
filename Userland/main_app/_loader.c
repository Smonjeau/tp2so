/*---------------------------------------------------------------------------------------------------
|   _LOADER.C    |	                                                                                |
|-----------------    		                                                                        |
| These functions handles messages received from the kernel, forwarding them to appropiate handlers.|
| It supports messages for starting the main app, and being notified of an exception or IRQ.        |
---------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <programs.h>
#include <syscalls.h>

extern char bss;
extern char endOfBinary;


int _start() {

	startProcess(shell, 0, (void*) 0,"shell");
	__asm__("hlt;\n\r");

	return 0;

}