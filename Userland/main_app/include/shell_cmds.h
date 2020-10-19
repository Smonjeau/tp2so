#ifndef SHELL_BUILTINS_H
#define SHELL_BUILTINS_H

// MISCELANEOUS COMMANDS

void printHelp(void);
void printWarning(void);
void printTime(void);
void divZeroException(void);
void invOpcodeException(void);

void displayImage(char *name, int x, int y);
void startFilter();
void startCat();
void line(int argc, char **argv);
void loop(int argc, char **argv);



// CPU COMMANDS

void printCPUTemp(void);
void printCPUInfo(void);
void printInfoReg(void);
void printStoredReg(void);


// MEMORY COMMANDS

void printMemDump(char * start);
void printMemStatus(void);


// PROCESSES COMMANDS

void printProcData();
void killProcess(char * pid);
void blockProcess(char * pid);
void niceProcess(char * pid, char * priority);

void printPipeInfo();
void printSemStatus();


// TEST COMMANDS

void test_mm(int argc, char **argv);
void test_proc(int argc, char **argv);
void test_sync();
void test_no_sync();

#endif