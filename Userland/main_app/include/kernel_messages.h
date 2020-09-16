/*---------------------------------------------------------------------------------------------------
|   KERNEL_MESSAGES.H    |                                                                          |
|----------------------                                                                             |
| This file defines numbers that are used by the kernel to send messages to userspace apps.  	    |
---------------------------------------------------------------------------------------------------*/

#define START 0
#define IRQ_PRODUCED 1
#define EXCEPTION_PRODUCED 2

#define TIMER_TICK_IRQ 0
#define KEYBOARD_IRQ 1

#define DIV_ZERO_EXCEPTION 0
#define INV_OPCODE_EXCEPTION 6