#include <stdint.h>

// Windows definitions

typedef struct Cursor{
    int x, y;

    int fontSize, fontColor;

    int withIndicator;
} Cursor;


typedef struct Window{
    int xi, xf, yi, yf;
    int textBackground;
    
    Cursor cursors[3];
    int activeCursor;
    
} Window;

void setWindow(Window *window);


// Draw definitions

void drawPoint(int x, int y, int size, int rgb);

void drawChar(int x, int y, char c, int size, int rgb);


// Text definitions

void clearLine();

void clearScreen();

void drawCursor(int color);

void printChar(char c);

void print(char c[]);

void printLine(char c[]);

void newLine();

void cleanBuffer(char *, int len);

void scrollUp(int dy);


// Image definitions

void drawImage(int xi, int yi, uint8_t *image, int height, int width);


// -------------------------- LISTENERS ---------------------

void winTimerTickListener();


// -------------------------- SHORTCUTS ---------------------

#define NULL ((void *) 0)

// -------------------------- CURSORS ---------------------

#define titleCursor 0
#define bodyCursor 1

// -------------------------- UI ELEMENTS ---------------------

#define titleX 0
#define titleY 0
#define titleSize 2
#define titleColor 0x00FF00

#define indicatorX 0
#define indicatorY 20
#define indicatorWidth 150
#define indicatorHeight 2
#define indicatorColor 0x00FF00

#define bodyY 50
#define bodyX 10
#define bodySize 1
#define bodyColor 0xFFFFFF
