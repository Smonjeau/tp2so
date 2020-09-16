/*---------------------------------------------------------------------------------------------------
|   TEST_WINDOW.C    |                                                                              |
|----------------------                                                                             |
| This windows will run tests on all the syscalls and some library functions. Some of the tests     |
| will automatically indicate if they passed, and some others will display information useful for   |
| evaluating the behaviour of the system.                                   				        |
---------------------------------------------------------------------------------------------------*/

#include <programs.h>
#include <windows_lib.h>
#include <std_lib.h>
#include <syscalls.h>
#include <asm_lib.h>

/* --------------------------------------------------------------------------------------------------------------------------
                                        		DEFINITIONS
------------------------------------------------------------------------------------------------------------------------- */

#define cursor 0

static Window w;

void regDumpTestSet();
void invalidOpcode();

static void testDraw();
static void testRegDump();
static void testMemDump();
static void testGetTime();
static void testCPUTemp();
static void testCPUInfo();
static void testGetRegBkp();

static void testInvalidOpcode();
static int testDivByZero(int n);


/* --------------------------------------------------------------------------------------------------------------------------
                                        		METHODS
------------------------------------------------------------------------------------------------------------------------- */


static void createWindow(){

	ScreenRes res;
	getRes(&res);

	w.xi = 0; w.xf = res.width;
    w.yi = 0; w.yf = res.height;

	w.cursors[cursor].x=0;	w.cursors[cursor].y=0;
	w.cursors[cursor].fontColor=0xFFFFFF;	w.cursors[cursor].fontSize=2;

    setWindow(&w);

}


int test(){

    createWindow();
	w.activeCursor = cursor;

    //testDivByZero(0);
    //testInvalidOpcode();

    testDraw();
    testRegDump();
    testMemDump();
    testGetTime();
    testCPUTemp();
    testCPUInfo();
    testGetRegBkp();

    while(1);

    return 1;

}


static void testDraw(){

    if(draw(0, 0, 0)==0 && draw(-1, 0, 0)==-1)
        printf("Draw validation passed\\n", 0);
    else
        printf("Draw validation failed\\n", 0);

}


static void testRegDump(){

    printf("RegDump test\\n", 0);


    // ------------ RIP -------------------

    RegDump dump;
    RegDump dump2;
    RegDump dump3;

    regDump(&dump);
    regDump(&dump2);
    __asm("nop");
    regDump(&dump3);
    
    int sizeOfRegDump = dump2.rip - dump.rip;
    if(dump3.rip - dump2.rip == sizeOfRegDump + 1)
        printf("- RIP passed\\n", 0);
    else
        printf("- RIP failed\\n", 0);


    // ------------ RBP/RSP -------------------

    regDump(&dump);
    __asm("push 1");
    regDump(&dump2);
    __asm("pop %rax");

    if(dump.rsp - dump2.rsp == 8 && dump.rbp == dump2.rbp)
        printf("- RSP/RBP passed\\n", 0);
    else
        printf("- RSP/RBP failed\\n", 0);


    // ------------- GPR ------------------
    
    regDumpTestSet();
    regDump(&dump);

    printf("RAX: %x\\n", 1, dump.rax);

    if(dump.rbx==2 && dump.rcx==3 && dump.rdx==4)
        printf("- GPR passed\\n", 0);
    else
        printf("- GPR failed\\n", 0);
   

}


static void testMemDump(){

    // Reserve 33 bytes and differ the last one
    char ptr[33], ptrDmp[33];
	ptrDmp[32] = ptr[32]+1;

    // Do a dump of 32 bytes
	memDump(ptr, ptrDmp);
	
    // Check that exactly 32 bytes are equal between addresses
	int i;
	for(i=0; ptr[i]==ptrDmp[i]; i++);
    if(i==32)
        printf("\\nMemDump test passed\\n", 0);
    else
        printf("\\nMemDump test failed\\n", 0);

}


static void testGetTime(){

    Time time;
	getTime(&time);

    if(time.hours>=0 && time.hours<=23 && time.minutes>=0 && time.minutes<=59 && time.seconds>=0 && time.seconds<=60)
        printf("\\nGetTime test passed\\n", 0);
    else
        printf("\\nGetTime test failed\\n", 0);

}


static void testCPUInfo(){

    CPUInfo info;

    char brandName[50], brandDesc[70];
	info.brandName = (char *) &brandName;
    info.brandDesc = (char *) &brandDesc;

    cpuInfo(&info);

    if(strcmp(info.brandName, "QEMU Virtual CPU version 2.5+") == 1 && strcmp(info.brandDesc, "This processor does not support the brand identification feature") == 1)
        printf("\\nCPUInfo test passed\\n", 0);
    else
        printf("\\nCPUInfo test failed\\n", 0);

}


static void testCPUTemp(){
    
    int temp = cpuTemp();

    if(temp>24)
        printf("\\nCPUTemp test passed\\n", 0);
    else
        printf("\\nCPUTemp test failed\\n", 0);

}


static void testGetRegBkp(){

    RegBkp bkp;
    getRegBkp(&bkp);
    printf("RAX: %x - RBX: %x - RCX: %x, - RDX: %x\\n", 4, bkp.rax, bkp.rbx, bkp.rcx, bkp.rdx);
    
}


static void testInvalidOpcode(){

    invalidOpcode();

}


// The argument n was placed to avoid compiler warning of div by zero

static int testDivByZero(int n){

    return 2/n;

}