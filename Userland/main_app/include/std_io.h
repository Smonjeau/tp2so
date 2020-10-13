// The following keys are represented with non used ASCII codes

#define NA 0        // NA = Not Ascii (unmapped keys, error codes)
#define F1 1
#define F2 2
#define F3 3
#define ESC 4

// The following are the standard file descriptors for input and output of a process

#define STDIN 0
#define STDOUT 1

// The following are syscall wrappers for reading/writing to stdin/stdout

char getChar();
int putChar(char c);