#include <stdint.h>

typedef struct RegDump{
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, rip, r8, r9, r10, r11, r12, r13, r14, r15;
} RegDump;

void regDump(RegDump *dump);

void halt();

void sti();