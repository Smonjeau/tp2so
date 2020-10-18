/*---------------------------------------------------------------------------------------------------
|   KERNEL_MESSAGES.H    |                                                                          |
|----------------------                                                                             |
| This file defines numbers that are used by the kernel to send messages to userspace apps.         |
| Also, it defines the address at which the main app of userspace is loaded.                  	    |
---------------------------------------------------------------------------------------------------*/

#define START_SHELL 0
#define EXCEPTION_PRODUCED 2

#define DIV_ZERO_EXCEPTION 0
#define INV_OPCODE_EXCEPTION 6

static void * const mainApp = (void*)0x400000;

typedef int (*EntryPoint)(int message, int nargs, ...);