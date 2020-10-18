#include <stdint.h>

// General definitions

void drawPoint(int x, int y, int size, int rgb);

void drawChar(int x, int y, char c);


// Text definitions

void printChar(char c);

void print(char c[]);

void cursorTickListener();


// Image definitions

void drawImage(int xi, int yi, uint8_t *image, int height, int width);