// MEMORY METHODS

void * memcpy(void * destination, const void * source, int length);

// CHAR METHODS

int isAlpha(char);

int isDigit(char);

int isHexa(char);

int isDecimalPoint(char);

int isSign(char);

int isOperator(char c);

int isParenthesis(char c);

int isSpace(char);

int isLower(char);

int isUpper(char);

char toLower(char);

int isPrintableChar(char c);

// CALCULATOR CONSTS

#define ADD '+'
#define SUBS '-'
#define MULT 'x'
#define DIV '%'
#define PRTH_OP '('
#define PRTH_CL ')'

// STRING METHODS

#define DTOA_FLOAT_MAX_LEN 4
#define EPSILON 0.0001

void reverseStr(char str[], int length);

char *itoa(int num, char *str, int base, int fixLen);

char *dtoa(double num, char *str);

void printf(char *format, int nargs, ...);

int strcmp(char *str1, char *str2);

int strncmp(char *s1, char *s2, int length);

void strcpy(char *src, char *dest);