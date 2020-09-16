/*---------------------------------------------------------------------------------------------------
|   EXCEPTIONS_DISPATCHER.C    |                                                                    |
|-------------------------------                                                                    |
| This function is part of the exception attending routine, it dispatches the exception to a    	|
| userspace callback, where a warning may be shown, and further actions taken.						|
---------------------------------------------------------------------------------------------------*/

#include <kernel_messages.h>

void exceptionDispatcher(int id, void * eip) {

	((EntryPoint) mainApp)(EXCEPTION_PRODUCED, 2, id, eip);

}