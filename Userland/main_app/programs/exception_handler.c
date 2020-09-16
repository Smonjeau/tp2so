/*---------------------------------------------------------------------------------------------------
|   EXCEPTION_HANDLER.C    |                                                                        |
|---------------------------                                                                        |
| This windows display exception information when they are produced. This includes common registers |
| at the moment of the exception, and which was the exception produced.		    			        |
---------------------------------------------------------------------------------------------------*/

#include <programs.h>
#include <windows_lib.h>
#include <std_lib.h>
#include <syscalls.h>
#include <keyboard_lib.h>
#include <kernel_messages.h>

/* --------------------------------------------------------------------------------------------------------------------------
                                        		DEFINITIONS
------------------------------------------------------------------------------------------------------------------------- */

#define cursor 0

static Window w;


/* --------------------------------------------------------------------------------------------------------------------------
                                        		METHODS
------------------------------------------------------------------------------------------------------------------------- */


static void createWindow(){

	ScreenRes res;
	getRes(&res);

	w.xi = 0; w.xf = res.width;
    w.yi = 0; w.yf = res.height;

    w.textBackground = 0x0000FF;

	w.cursors[cursor].x=0;	w.cursors[cursor].y=0;
	w.cursors[cursor].fontColor=0xFFFFFF;	w.cursors[cursor].fontSize=2;

    setWindow(&w);

}


int exception(va_list args){

    int id = va_arg(args, int);
    void * eip = va_arg(args, void *);

    createWindow();
	w.activeCursor = cursor;

    for(int x=0; x<w.xf-w.xi; x++){
        for(int y=0; y<w.yf-w.yi; y++){
            draw(x, y, 0x0000FF);
        }
    }

    switch(id){
        case DIV_ZERO_EXCEPTION:
            printf("Division by zero detected!\\n", 0);
            printf("Possible causes: \\n", 0);
            printf(" - You didn't finish elementary school\\n", 0);
            break;
        case INV_OPCODE_EXCEPTION:
            printf("Invalid opcode detected!\\n", 0);
            printf("Possible causes: \\n", 0);
            printf(" - You received an EMP attack\\n", 0);
            printf(" - You tried to destroy the machine\\n", 0);
            break;
    }

    RegBkp bkp;
    getRegBkp(&bkp);

    printf("\\nFeed this information to the developers:", 0);
    printf("\\nRAX: %x - RBX: %x - RCX: %x - RDX: %x", 4, bkp.rax, bkp.rbx, bkp.rcx, bkp.rcx);
    printf("\\nRIP: %x - RBP: %x - RSI: %x - RDI: %x", 4, eip, bkp.rbp, bkp.rsi, bkp.rdi);
    printf("\\n\\nPress enter to restart the system", 0);

    while(1){
        if(getChar()==13){
            for(int x=w.xi; x<w.xf; x++){
                for(int y=w.yi; y<w.yf; y++){
                    draw(x, y, 0x000000);
                }
            }
            main(START);
        }
    }

    return 1;
    
}