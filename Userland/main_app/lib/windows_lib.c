/*---------------------------------------------------------------------------------------------------
|   WINDOWS_LIB.C    |                                                                              |
|---------------------                                                                              |
| This libary implements all the needed functions for a window to work, such as methods for drawing |
  and writing on its own space. Also, it handles the work of managing which is the selected screen  |
---------------------------------------------------------------------------------------------------*/

#include <windows_lib.h>
#include <std_lib.h>
#include <font.h>
#include <syscalls.h>

/* --------------------------------------------------------------------------------------------------------------------------
                                            WINDOWS METHODS
-------------------------------------------------------------------------------------------------------------------------- */

static Window *window;

void setWindow(Window *currWindow)
{
    window = currWindow;
}

#define cursor window->cursors[window->activeCursor]

#define wheight window->yf - window->yi

#define wwidth window->xf - window->xi

/* --------------------------------------------------------------------------------------------------------------------------
                                            DRAW METHODS
-------------------------------------------------------------------------------------------------------------------------- */

#define screenDataMax 1500

// Holds the colors written on each position of the screen
int screenData[screenDataMax][screenDataMax];

void drawPoint(int x, int y, int size, int rgb)
{

    int absx = window->xi + x;
    int absy = window->yi + y;

    screenData[absx][absy] = rgb;

    for (int x = absx; x < absx + size; x++)
    {
        for (int y = absy; y < absy + size; y++)
        {
            draw(x, y, rgb);
        }
    }
}

void drawChar(int x, int y, char c, int size, int rgb)
{

    for (int col = 0; col < fontWidth; col++)
    {
        for (int row = 0; row < fontHeight; row++)
        {

            int draw = font8x8_basic[(int)c][col] & 1 << row;

            drawPoint(x + row * size, y + col * size, size, draw ? rgb : window->textBackground);

        }
    }
}

/* --------------------------------------------------------------------------------------------------------------------------
                                            TEXT METHODS
-------------------------------------------------------------------------------------------------------------------------- */

#define charSpacing 1;
#define lineSpacing 1;

void scrollUp(int dy){

    ScreenRes res;
    getRes(&res);

    for(int y=bodyY; y<res.height-dy; y++){
        for(int x=window->xi; x<window->xf; x++){
            screenData[x][y] = screenData[x][y+dy];
            draw(x, y, screenData[x][y]);
        }
    }

    for(int y=res.height-dy; y<res.height; y++){
        for(int x=window->xi; x<window->xf; x++){
            screenData[x][y] = 0;
            draw(x, y, 0);
        }
    }

    cursor.y = cursor.y-dy >=bodyY ? cursor.y-dy : bodyY;

}


void scrollUntilFit(){
    int charHeight = cursor.fontSize * fontHeight + lineSpacing;
    int lowestY = cursor.y + charHeight;
    int lowestAllowedY = wheight - 4*charHeight;

    if (lowestY > lowestAllowedY)
    {
        scrollUp(lowestY - lowestAllowedY);
    }
}


void clearScreen()
{

    for (int x = window->xi; x < window->xf; x++)
    {
        for (int y = window->yi; y < window->yf; y++)
        {
            drawPoint(x, y, 1, 0);
        }
    }
}


void clearLine()
{

    for (int x = window->xi; x < (window->xf - fontHeight * cursor.fontSize); x++)
    {
        drawPoint(x, cursor.y, fontHeight * cursor.fontSize, 0);
    }

    cursor.x = 0;
}


static void nextChar()
{

    cursor.x += cursor.fontSize * fontWidth + charSpacing;

    if (cursor.x > wwidth)
    {
        cursor.x = 0;
        cursor.y += cursor.fontSize * fontHeight + lineSpacing;
    }

    scrollUntilFit();

}


static void prevChar()
{

    cursor.x -= cursor.fontSize * fontWidth + charSpacing;

    if (cursor.x < 0)
    {
        cursor.x = 0;
    }
}


void newLine()
{

    cursor.x = 0;
    cursor.y += cursor.fontSize * fontWidth + lineSpacing;

    scrollUntilFit();

}


void drawCursor(int color){

    for (int y=0; y<fontHeight; y++){
        drawPoint(cursor.x+2, cursor.y+y, 2, color);
    }

}


int printing=0;

void printChar(char c)
{

    printing=1;

    drawCursor(0);

    switch(c){

        case '\r':
            newLine();
            break;

        case '\b':
            prevChar();
            drawPoint(cursor.x, cursor.y, cursor.fontSize * fontHeight, 0);
            break;

        default:
            if (isPrintableChar(c)){
                drawChar(cursor.x, cursor.y, c, cursor.fontSize, cursor.fontColor);
                nextChar();
            }

    }    

    printing=0;

}


void print(char s[])
{

    for (int i = 0; s[i] != 0; i++)
        printChar(s[i]);
}


void printLine(char s[])
{

    print(s);
    newLine();
}


void cleanBuffer(char *buffer, int len)
{

    for (int j = 0; j < len; j++)
        buffer[j] = 0;
}


/* --------------------------------------------------------------------------------------------------------------------------
                                            IMAGE METHODS
-------------------------------------------------------------------------------------------------------------------------- */

void drawImage(int xi, int yi, uint8_t *image, int width, int height){

    int scaleRed = 256/8;
    int scaleGreen = 256/8;
    int scaleBlue = 256/4;

    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){

            int pos = (y*width)+x;
            //printf("%d: %d\\n", 2, pos, image[pos]);

            uint8_t r = ((image[pos] & 0xE0) >> 5) * scaleRed;
            uint8_t g = ((image[pos] & 0x1C) >> 2) * scaleGreen;
            uint8_t b = ((image[pos] & 0x03) >> 0) * scaleBlue;

            //printf("%d - R: %d, G: %d, B: %d\\n", 4, pos, r, g, b);

            uint32_t rgb = (r << 16) + (g << 8) + b;
            
            drawPoint(xi+x, yi+y, 1, rgb);

        }
    }

}



/* --------------------------------------------------------------------------------------------------------------------------
                                            LISTENERS
-------------------------------------------------------------------------------------------------------------------------- */

static int ticks=0;

int cursorActive = 1;

void winTimerTickListener(){
    ticks++;

    if(printing)
        return;

    if(! cursor.withIndicator)
        return;

    if((ticks%15) == 0 ){
        drawCursor(cursorActive ? 0xFFFFFF : 0x000000);
        cursorActive = cursorActive ? 0 : 1;
    }

}

int getTicks(){
    return ticks;
}