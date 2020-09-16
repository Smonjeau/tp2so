/*---------------------------------------------------------------------------------------------------
|   INT_HANDLER.C    |                                                                              |
|---------------------                                                                              |
| These functions handles IRQs received from the kernel, calling appropiate callbacks               |
---------------------------------------------------------------------------------------------------*/

#include <programs.h>
#include <windows_lib.h>
#include <keyboard_lib.h>
#include <kernel_messages.h>

int interruption(va_list valist){

    int id = va_arg(valist, int);

    if(id == TIMER_TICK_IRQ){
        winTimerTickListener();
    }

    if(id == KEYBOARD_IRQ){
        int keyPos = va_arg(valist, int);
        libKeyboardListener(keyPos);
    }

    return 1;

}