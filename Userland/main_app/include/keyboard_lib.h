// Backspace: 14, Tab: 15, Enter: 28, Ctrl: 29, LShift: 42/170, RShift: 54/182, Alt: 56, Space: 57, F1-F10: 59-68
// Cmd: 91, LA: 224/75, RA: 224/77

#define TAB 15
#define ENTER 28
#define BACKSPACE 14

char getChar();

void libKeyboardListener(int keyPos);


// These special codes have been defined for representing special key as ASCII characters that wouldn't be used in this app otherwise

#define f1Code 1
#define f2Code 2
#define f3Code 3
#define escCode 4