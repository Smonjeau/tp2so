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

extern void _hlt();
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


void test2(int argc, char **argv){
	for(int y=512; y<1024; y++)
		for(int x=0; x<1024; x++)
			for(double c=0; c<9999; c++)
				draw(x, y, 0x00FF00);
	while(1);
}

void test1(int argc, char **argv){
	createProcessContext(0, (void*)0, test2);

	for(int y=0; y<512; y++)
		for(int x=0; x<1024; x++)
			for(double c=0; c<9999; c++)
				draw(x, y, 0xFF0000);
	while(1);
}

int main(){

	load_idt();

	// createProcessContext(0, (void*)0, test1);

	// while(1);

	((EntryPoint) mainApp)(START_SHELL, 0);
		
	return 0;

}