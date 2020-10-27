// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*---------------------------------------------------------------------------------------------------
|   SYSCALLS_SCREEN.C    |                                                                          |
|--------------------                                                                               |
| These functions implement syscalls related to the screen.   	                                    |
---------------------------------------------------------------------------------------------------*/

#include <screen_driver.h>

/* ---------------------------------------------------------------------------------------------------------------------------
                                                    DRAW
--------------------------------------------------------------------------------------------------------------------------- */

int sysDraw(int cx, int cy, int rgb){

    return draw(cx, cy, rgb);

}

/* ---------------------------------------------------------------------------------------------------------------------------
                                                    GETRES
--------------------------------------------------------------------------------------------------------------------------- */

typedef struct ScreenRes{
    int height;
    int width;
} ScreenRes;


int sysGetRes(ScreenRes * res){
    
    res->height = getResHeight();
    res->width = getResWidth();

    return 0;

}