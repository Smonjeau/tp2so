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


int main(){

	load_idt();

	((EntryPoint) mainApp)(START, 0);
		
	return 0;

}