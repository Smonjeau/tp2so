#ifndef SHELL_BUILTINS_H
#define SHELL_BUILTINS_H

// MISCELANEOUS COMMANDS

void clearScreen(int argc, char **argv);
void printWarning(int argc, char **argv);
void printHelp(int argc, char **argv);

void printTime(int argc, char **argv);
void divZeroException(int argc, char **argv);
void invOpcodeException(int argc, char **argv);

void displayImage(int argc, char **argv);
void filter(int argc, char **argv);
void cat(int argc, char **argv);
void wc(int argc, char **argv);
void line(int argc, char **argv);
void loop(int argc, char **argv);


// CPU COMMANDS

void printCPUTemp(int argc, char **argv);
void printCPUInfo(int argc, char **argv);
void printInfoReg(int argc, char **argv);
void printStoredReg(int argc, char **argv);


// MEMORY COMMANDS

void printMemDump(int argc, char **argv);
void printMemStatus(int argc, char **argv);


// PROCESSES COMMANDS

void printProcData();
void killProcess(int argc, char **argv);
void blockProcess(int argc, char **argv);
void niceProcess(int argc, char **argv);

void printPipeInfo(int argc, char **argv);
void printSemStatus(int argc, char **argv);

typedef void (*ProcMain)(int,char**);

void pipeLeftProc(ProcMain main, int argc, char **argv, char *name, int fds[2]);
void pipeRightProc(ProcMain main, int argc, char **argv, char *name, int fds[2]);


// TEST COMMANDS

void test_mm(int argc, char **argv);
void test_proc(int argc, char **argv);
void test_prio(int argc, char **argv);
void test_sync(int argc, char **argv);
void test_no_sync(int argc, char **argv);

#endif