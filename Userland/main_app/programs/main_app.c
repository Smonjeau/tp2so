/*---------------------------------------------------------------------------------------------------
|   MAIN_APP.C    |                                                                              	|
|----------------------                                                                             |
| This app will alternate between two windows, starting with the first one.			  				|
---------------------------------------------------------------------------------------------------*/

#include <programs.h>
#include <syscalls.h>
#include <windows.h>
#include <kernel_messages.h>

int main(){

	// test();

	initWindow1();
	initWindow2();

	while(1){
		window1();
		window2();
	}
	
	return 1;

}