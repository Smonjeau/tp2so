#include <syscalls.h>
#include <asm_lib.h>
#include <std_lib.h>
#include <shell_cmds.h>


void printCPUTemp(int argc, char **argv){
	int temp = cpuTemp();
	printf("\n Computer's Temperature: %d\n", 0, 1, temp);

	kill(-1);
}


void printCPUInfo(int argc, char **argv){
	CPUInfo info;

	char brandName[50], brandDesc[70];
	info.brandName = (char *)&brandName;
	info.brandDesc = (char *)&brandDesc;

	cpuInfo(&info);

	printf("\nBrand name: %s", 1, info.brandName);
	printf("\nBrand description: %s\n", 0, 1, info.brandDesc);

	kill(-1);
}


void printInfoReg(int argc, char **argv){
	RegDump reg;
	regDump(&reg);

	printf("Register's values:\n", 0);
	printf("--- --- --- --- --- --- --- --- --- --- --- --- ---\n", 0);
	printf("\n - rax - %x", 1, reg.rax);
	printf("\n - rbx - %x", 1, reg.rbx);
	printf("\n - rcx - %x", 1, reg.rcx);
	printf("\n - rdx - %x", 1, reg.rdx);
	printf("\n - rsi - %x", 1, reg.rsi);
	printf("\n - rdi - %x", 1, reg.rdi);
	printf("\n - rbp - %x", 1, reg.rbp);
	printf("\n - rsp - %x", 1, reg.rsp);

	printf("\n - r8 - %x", 1, reg.r8);
	printf("\n - r9 - %x", 1, reg.r9);
	printf("\n - r10 - %x", 1, reg.r10);
	printf("\n - r11 - %x", 1, reg.r11);
	printf("\n - r12 - %x", 1, reg.r12);
	printf("\n - r13 - %x", 1, reg.r13);
	printf("\n - r14 - %x", 1, reg.r14);
	printf("\n - r15 - %x\n", 0, 1, reg.r15);

	printf("--- --- --- --- --- --- --- --- --- --- --- --- ---\n", 0);

	kill(-1);
}


void printStoredReg(int argc, char **argv){
	RegBkp reg;
	getRegBkp(&reg);

	printf("Stored register's values:\n", 0);
	printf("--- --- --- --- --- --- --- --- --- --- --- --- ---\n", 0);
	printf("\n - rax - %x", 1, reg.rax);
	printf("\n - rbx - %x", 1, reg.rbx);
	printf("\n - rcx - %x", 1, reg.rcx);
	printf("\n - rdx - %x", 1, reg.rdx);
	printf("\n - rsi - %x", 1, reg.rsi);
	printf("\n - rdi - %x", 1, reg.rdi);
	printf("\n - rbp - %x", 1, reg.rbp);

	printf("\n - r8 - %x", 1, reg.r8);
	printf("\n - r9 - %x", 1, reg.r9);
	printf("\n - r10 - %x", 1, reg.r10);
	printf("\n - r11 - %x", 1, reg.r11);
	printf("\n - r12 - %x", 1, reg.r12);
	printf("\n - r13 - %x", 1, reg.r13);
	printf("\n - r14 - %x", 1, reg.r14);
	printf("\n - r15 - %x\n", 0, 1, reg.r15);

	printf("--- --- --- --- --- --- --- --- --- --- --- --- ---\n", 0);

	kill(-1);
}