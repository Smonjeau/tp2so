/*---------------------------------------------------------------------------------------------
|   VIDEO_LIB.C    |                                                                          |
|---------------------                                                                        |
| This libary implements helper functions for using the video mode, such as for drawing and   |
| printing text                                                                               |
---------------------------------------------------------------------------------------------*/

#include <inttypes.h>
#include <font.h>
#include <screen_driver.h>
#include <video_lib.h>

#define SCREEN_DATA_MAX 1500

#define TEXT_COLOR 0xFFFFFF
#define TEXT_SIZE 1
#define TEXT_HOR_SPACING 1
#define TEXT_VER_SPACING 1

#define BACK_COLOR 0x000000

#define WIDTH getResWidth()
#define HEIGHT getResHeight()


int curX, curY;

// Holds the colors written on each position of the screen
int screenData[SCREEN_DATA_MAX][SCREEN_DATA_MAX];


/* --------------------------------------------------------------------------------------------------------------------------
                                            GENERAL METHODS
-------------------------------------------------------------------------------------------------------------------------- */


void drawPoint(int x, int y, int size, int rgb){

    screenData[x][y] = rgb;

    for(int cx=x; cx<x+size; cx++)
        for (int cy=y; cy<y+size; cy++)
            draw(cx, cy, rgb);

}


/* --------------------------------------------------------------------------------------------------------------------------
                                            TEXT METHODS
-------------------------------------------------------------------------------------------------------------------------- */

void clearScreen(){

    for (int x=0; x<WIDTH; x++)
        for (int y=0; y<HEIGHT; y++)
            drawPoint(x, y, 1, 0);

    curX=0; curY=0;

}


void scrollUp(int dy){

    for(int y=0; y<HEIGHT-dy; y++){
        for(int x=0; x<WIDTH; x++){
            screenData[x][y] = screenData[x][y+dy];
            draw(x, y, screenData[x][y]);
        }
    }

    for(int y=HEIGHT-dy; y<HEIGHT; y++){
        for(int x=0; x<WIDTH; x++){
            screenData[x][y] = 0;
            draw(x, y, 0);
        }
    }

    curY = curY-dy >= 0 ? curY-dy : 0;

}


void scrollUntilFit(){

    int charHeight = TEXT_SIZE * fontHeight + TEXT_VER_SPACING;

    int lowestY = curY + charHeight;
    int lowestAllowedY = HEIGHT - 4*charHeight;

    if (lowestY > lowestAllowedY)
        scrollUp(lowestY - lowestAllowedY);

}


void nextChar(){

    curX += TEXT_SIZE * fontWidth + TEXT_HOR_SPACING;

    if (curX > WIDTH){
        curX = 0;
        curY += TEXT_SIZE * fontHeight + TEXT_VER_SPACING;
    }

    scrollUntilFit();

}


void prevChar(){

    curX -= TEXT_SIZE * fontWidth + TEXT_HOR_SPACING;

    if (curX < 0)
        curX = 0;

}


void nextLine(){

    curX = 0;
    curY += TEXT_SIZE * fontWidth + TEXT_VER_SPACING;

    scrollUntilFit();

}


void drawChar(int x, int y, char c){

    for (int col = 0; col < fontWidth; col++){
        for (int row = 0; row < fontHeight; row++){

            int draw = font8x8_basic[(int)c][col] & 1 << row;

            if(draw)
                drawPoint(x + row*TEXT_SIZE, y + col*TEXT_SIZE, TEXT_SIZE, TEXT_COLOR);

        }
    }

}


int printing=0;

void printChar(char c){

    printing=1;

    for (int y=0; y<fontHeight; y++)
        drawPoint(curX+2, curY+y, 2, 0x000000);

    switch(c){

        case '\n':
            nextLine();
            break;

        case '\b':
            prevChar();
            drawPoint(curX, curY, TEXT_SIZE * fontHeight, 0);
            break;

        case '\f':
            clearScreen();
            break;

        default:
            drawChar(curX, curY, c);
            nextChar();

    }    

    printing=0;

}


void print(char s[]){

    for (int i = 0; s[i] != 0; i++)
        printChar(s[i]);

}


static int ticks=0;

int cursorActive = 1;

void cursorTickListener(){
  
    ticks++;

    if(printing)
        return;

    if((ticks%15) == 0 ){
        for (int y=0; y<fontHeight; y++)
            drawPoint(curX+2, curY+y, 2, cursorActive ? 0xFFFFFF : 0x000000);
        cursorActive = cursorActive ? 0 : 1;
    }

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
            //printf("%d: %d\n", 2, pos, image[pos]);

            uint8_t r = ((image[pos] & 0xE0) >> 5) * scaleRed;
            uint8_t g = ((image[pos] & 0x1C) >> 2) * scaleGreen;
            uint8_t b = ((image[pos] & 0x03) >> 0) * scaleBlue;

            //printf("%d - R: %d, G: %d, B: %d\n", 4, pos, r, g, b);

            uint32_t rgb = (r << 16) + (g << 8) + b;
            
            drawPoint(xi+x, yi+y, 1, rgb);

        }
    }

}