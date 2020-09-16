/*---------------------------------------------------------------------------------------------------
|   IRQ_DISPATCHER.C    	   |                                                                    |
|-------------------------------                                                                    |
| These functions are part of the IRQs attending routines, they dispatch the exceptions to	    	|
| userspace callbacks, where further actions may be taken.											|
---------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <keyboard_driver.h>
#include <kernel_messages.h>


static void int_20();

static void int_21();


void irqDispatcher(uint64_t irq) {

	switch (irq) {

		case 0:			// Timer tick
			int_20();
			break;

		case 1:			// Keyboard
			int_21();
			break;

	}

	return;

}


// RTC
void int_20() {

	((EntryPoint) mainApp)(IRQ_PRODUCED, 1, TIMER_TICK_IRQ);

}


// Keyboard
void int_21() {

	((EntryPoint) mainApp)(IRQ_PRODUCED, 2, KEYBOARD_IRQ, read());

}