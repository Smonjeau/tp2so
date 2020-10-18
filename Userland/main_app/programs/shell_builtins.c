#include <syscalls.h>
#include <asm_lib.h>
#include <std_lib.h>
#include <std_io.h>
#include <stdint.h>
#include <shell_builtins.h>



/* ----------------------------------------------------------------------------------------------------
						                MISCELLANEOUS
----------------------------------------------------------------------------------------------------- */

void printHelp(void){
	printf("---------------------------------------------------\n", 0);

	printf("- help              to go to the Help Manual\n", 0);
	printf("- clear             to clear the screen\n\n", 0);
	
	printf("- time              to get the system time\n", 0);
	printf("- divzero           to execute a Div by Zero\n", 0);
	printf("- invopcode         to exectue an invalid opcode\n\n", 0);
	
	printf("- display anon      to show an image of Anonymous\n", 0);
	printf("- display matrix    to show an image of Matrix\n\n", 0);

	printf("- cputemp           to get the CPU Temp\n", 0);
	printf("- cpuinfo           to get the CPU Brand Data\n", 0);
	printf("- inforeg           to get the Register's Values\n", 0);
	printf("- storedreg         to get the Stored Register's Values\n\n", 0);
	
	printf("- memdump n         to print memory starting at n\n", 0);
	printf("- mem          to print heap status\n\n", 0);

	printf("- ps                to list all processes\n", 0);
	printf("- kill pid          to kill a process\n", 0);
	printf("- block pid         to block a process\n", 0);
	printf("- nice pid priority to change priority (100 - 139)\n", 0);
	printf("- pipe              to list all pipes\n", 0);
    printf("- line              to draw a line (for testing)\n", 0);
    printf("- loop              to start loop (for testing)\n\n", 0);
		
	printf("---------------------------------------------------", 0);
}


void printTime(void){
	Time t;
	getTime(&t);

	printf("\nTime now: %2d:%2d:%2d\n", 0, 3, t.hours, t.minutes, t.seconds);
}


void divZeroException(void){
	int a;
	a = 2 / 0;
}


void invalidOpcode();

void invOpcodeException(void){
	invalidOpcode();
}


void printWarning(){
	printf(">> Error:\n", 0, 0);
	printf("Command not found\n", 0);
	printf("If you want to see the command manual type 'help'.\n", 0);
}


/* ----------------------------------------------------------------------------------------------------
						                CPU MANAGEMENT
----------------------------------------------------------------------------------------------------- */

void printCPUTemp(void){
	int temp = cpuTemp();
	printf("\n Computer's Temperature: %d\n", 0, 1, temp);
}


void printCPUInfo(void){
	CPUInfo info;

	char brandName[50], brandDesc[70];
	info.brandName = (char *)&brandName;
	info.brandDesc = (char *)&brandDesc;

	cpuInfo(&info);

	printf("\nBrand name: %s", 1, info.brandName);
	printf("\nBrand description: %s\n", 0, 1, info.brandDesc);
}


void printInfoReg(void){
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
}


void printStoredReg(void){
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
}


/* ----------------------------------------------------------------------------------------------------
						                MEMORY MANAGEMENT
----------------------------------------------------------------------------------------------------- */

void printMemDump(char *sourceStr){

	uint64_t sourceHex = 0;
	for (int i = 2; sourceStr[i]; i++){
		char c = '0';
		if (sourceStr[i] >= '0' && sourceStr[i] <= '9')
			c = '0';
		else if (sourceStr[i] >= 'a' && sourceStr[i] <= 'z')
			c = 'a';
		else
			c = 'A';
		sourceHex = sourceHex * 16 + sourceStr[i] - c;
	}

	if (sourceHex < 0){
		printf("Parameter not allowed\n", 0);
		return;
	}

	char *src = (void *) 0;
	src = (char *) sourceHex;
	char *dst = src + 32;

	memDump((void *)src, (void *)dst);

	putChar('\n');
	for (int i = 0; i < 33; i += 8){
		printf("%2x: %2x %2x %2x %2x %2x %2x %2x %2x\n", 0, 9, src + i, src[i], src[i + 1], src[i + 2], src[i + 3],
			   src[i + 4], src[i + 5], src[i + 6], src[i + 7]);
	}
	
}


void printMemStatus(){

	int total_mem=0;
    int  avail_mem=0;
    int occ_mem=0;

    memStatus(&total_mem,&avail_mem,&occ_mem);
    
    printf("Total memory: %d KB\n",1,total_mem);
    printf("Free memory: %d KB\n",1,avail_mem);
    printf("Occupied memory: %d KB\n",1,occ_mem);
    

}


/* ----------------------------------------------------------------------------------------------------
						                PROCESS MANAGEMENT
----------------------------------------------------------------------------------------------------- */

void printProcData(){
	char * buffer = malloc(90 * 40);
    if(buffer == (void *) 0)
		return;

    ps(buffer);    
    printf(buffer, 0);
    free(buffer);


}



void killProcess(char * pid){
	int _pid = atoi(pid);

	if(_pid==-1){
		printf("Argument must be a pid. Use ps to see processes\n", 0);
		return;
	}
    else if(_pid==0){
        printf("Cannot kill shell process\n", 0);
        return;
    }

	kill(_pid);
}


void blockProcess(char * pid){
	int _pid= atoi(pid);

	if(_pid==-1){
		printf("Argument must be a pid. Use ps to see processes\n", 0);
		return;
	} else if(_pid==0){
		printf("Can not block the shell", 0);
		return;
	} else if(_pid==1){
        printf("Dont kill the dummy proccess", 0);
        return;
    }

	block(_pid);
}

void printPipeInfo() {
	char * buffer = malloc(90 * 40);
    if(buffer == (void *) 0)
		return;

    pipeInfo(buffer);    
    printf(buffer, 0);
    free(buffer);
	
}

void niceProcess(char * pid, char * priority) {
	int _pid = atoi(pid);
	int _prio = atoi(priority);

	if(_pid==-1){
		printf("Argument must be a pid. Use ps to see processes\n", 0);
		return;
	}

	nice(_pid, _prio);
}
typedef struct Semaphore{
    int id;
    int value;
    int blockedPIDs[MAX_BLOCKED_PIDS];
    int blockedPIDsSize;
    int lock;
    struct Semaphore * next;
}  Semaphore;
void printSemStatus(){
    Semaphore * buffer = malloc(20*sizeof(struct Semaphore));
    int qty =0;
    semStatus(buffer,&qty);
    Semaphore sem;
    int i=0;
    printf("Semaphores status\n",0);
    while(i<qty){
        sem=*(buffer + i++);
        printf("ID:%d  Value:%d   There are %d processes blocked in this semaphore.\n",3,sem.id,sem.value,sem.blockedPIDsSize);
        
       	if(sem.blockedPIDsSize>0)
            printf("The blocked processes are:",0);
        for(int j=0;j<sem.blockedPIDsSize;j++)
            printf(" %d ",1,sem.blockedPIDs[j]);

    }
    free(buffer);
}