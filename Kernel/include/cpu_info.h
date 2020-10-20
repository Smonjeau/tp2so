#include <stdint.h>

typedef struct CPUInfo{
    char *brandName, *brandDesc;
} CPUInfo;

void getBrandName(char *brand);

int getBrandIndex();


typedef struct RegDump{
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, rip, r8, r9, r10, r11, r12, r13, r14, r15, eflags;
} RegDump;

void getRegisters(RegDump *dump);


int getTemperature();