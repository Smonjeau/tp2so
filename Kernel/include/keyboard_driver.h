#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H
#include <pipe_manager.h>

void handleKeyStroke();

char getChar();

int readStdin (char * buff, int dim);

#endif