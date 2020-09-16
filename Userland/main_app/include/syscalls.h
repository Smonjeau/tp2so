#include <stdint.h>

// ------------------------------------------------ DRAW -------------------------------------------------------------

int draw(int x, int y, int rgb);

// ------------------------------------------------ GETRES -------------------------------------------------------------

typedef struct ScreenRes{
    int height, width;
} ScreenRes;


int getRes(ScreenRes * res);

// ------------------------------------------------ MEMDUMP -------------------------------------------------------------

int memDump(void *src, void *dest);

// ------------------------------------------------ GETTIME -------------------------------------------------------------

typedef struct Time{
    int hours, minutes, seconds;
} Time;

int getTime(Time * time);

// ------------------------------------------------ CPUINFO -------------------------------------------------------------

typedef struct CPUInfo{
    char *brandName, *brandDesc;
} CPUInfo;

int cpuInfo(CPUInfo * info);

// ------------------------------------------------ CPUTEMP -------------------------------------------------------------

int cpuTemp();

// ------------------------------------------------ GETREGBKP ----------------------------------------------------------

typedef struct RegBkp{
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
} RegBkp;

int getRegBkp(RegBkp * dest);
