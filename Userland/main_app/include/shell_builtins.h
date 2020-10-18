#ifndef SHELL_BUILTINS_H
#define SHELL_BUILTINS_H

void printHelp(void);
void divZeroException(void);
void invOpcodeException(void);
void printTime(void);
void printWarning(void);

void printCPUTemp(void);
void printCPUInfo(void);
void printInfoReg(void);
void printStoredReg(void);

void printMemDump(char * start);
void printMemStatus(void);

void printProcData();
void printPipeInfo();
void killProcess(char * pid);
void blockProcess(char * pid);
void niceProcess(char * pid, char * priority);


void printSemStatus();

#define MAX_BLOCKED_PIDS 20

#endif