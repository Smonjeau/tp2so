#ifndef VIDEO_LIB_H
#define VIDEO_LIB_H

#include <stdint.h>
#include <pipe_manager.h>

// General definitions

void drawPoint(int x, int y, int size, int rgb);

void drawChar(int x, int y, char c);


// Text definitions

extern int backColor;

void printChar(char c);

void print(char c[]);

void cursorTickListener();


// Image definitions

void drawImage(int xi, int yi, uint8_t *image, int height, int width);

void assignStdoutPipe(pipe new);

#endif