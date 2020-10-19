// The following keys are represented with non used ASCII codes

#define EOT 4

// The following are the standard file descriptors for input and output of a process

#define STDIN 0
#define STDOUT 1

// The following are syscall wrappers for reading/writing to stdin/stdout

char getChar();

int putChar(char c);