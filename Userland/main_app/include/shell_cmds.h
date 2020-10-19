#ifndef SHELL_BUILTINS_H
#define SHELL_BUILTINS_H

// MISCELANEOUS COMMANDS

void printHelp(void);
void printWarning(void);
void printTime(void);
void divZeroException(void);
void invOpcodeException(void);

void displayImage(char *name);
void filter(int argc, char **argv);
void cat(int argc, char **argv);
void wc(int argc, char **argv);
void line(int argc, char **argv);
void loop(int argc, char **argv);



// CPU COMMANDS

void printCPUTemp(void);
void printCPUInfo(void);
void printInfoReg(void);
void printStoredReg(void);


// MEMORY COMMANDS

void printMemDump(int argc, char **argv);
void printMemStatus(void);


// PROCESSES COMMANDS

void printProcData();
void killProcess(int argc, char **argv);
void blockProcess(int argc, char **argv);
void niceProcess(char * pid, char * priority);

void printPipeInfo();
void printSemStatus();


// TEST COMMANDS

void test_mm(int argc, char **argv);
void test_proc(int argc, char **argv);
void test_prio(int argc, char **argv);
void test_sync(int argc, char **argv);
void test_no_sync(int argc, char **argv);

#endif