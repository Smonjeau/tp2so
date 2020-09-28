/*---------------------------------------------------------------------------------------------------
|   KERNEL.C    |                                                                          			|
|----------------                                                                             		|
| The main file of the kernel, here the loading of IDT and modules happens.         				|
| Also, the main app module is launched here.                 	    								|
---------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <lib.h>
#include <module_loader.h>
#include <idt_loader.h>
#include <screen_driver.h>
#include <kernel_messages.h>
#include <process_manager.h>
#include <mem_manager.h>

extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;


void clearBSS(void * bssAddress, uint64_t bssSize){
	memset(bssAddress, 0, bssSize);
}


void * getStackBase(){
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}


void * initializeKernelBinary(){
	void * moduleAddresses[] = {
		mainApp,
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}


int test1(int argc, char **argv){
	static int x1=0;
	static int y1 = 200;
	//while(1) {
		x1=0;
		for(; x1<1024; x1++)
			draw(x1, y1, 0xFF0000);
		y1 += 1;
		while(1);

	//}
	
	


}

int test2(int argc, char **argv){
	static int x2=0;
	static int y2 = 300;
	//while(1) {
		x2=0;
		for(; x2<1024; x2++)
			draw(x2, y2, 0x00FF00);
		y2 += 1;
		//while(1);
		killProcess(-1);
		
	//}
	
}

int test3(int argc, char **argv){
	static int x3=0;
	static int y3 = 400;
	//while(1) {
		x3=0;
		for(; x3<1024; x3++)
			draw(x3, y3, 0x0000FF);
		y3 += 1;
		while(1);
	//}
	
}


int main(){

	load_idt();

	createProcessContext(0, (void*)0, test1);
	createProcessContext(0, (void*)0, test2);
	createProcessContext(0, (void*)0, test3);
	

	__asm__("sti\n\t");

	((EntryPoint) mainApp)(START_SHELL, 0);
		
	return 0;

}

/*
test1: RSP 0x110f48 RIP 0x101308
test2: RSP 0x110ea8 RIP 0x10134e
*/