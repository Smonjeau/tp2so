/*---------------------------------------------------------------------------------------------------
|   EXCEPTIONS_DISPATCHER.S    |                                                          			|
|-------------------------------                                                          			|
| This file provides exception handling routines.											    	|
---------------------------------------------------------------------------------------------------*/

.global _exception0Handler
.global _exception6Handler

.extern exceptionDispatcher

.intel_syntax noprefix

.section .text

/* --------------------------------------------------------------------------------------------------------------------------------------------------
															MACROS
-------------------------------------------------------------------------------------------------------------------------------------------------- */

.include "./interruptions/macros.s"

.macro exceptionHandler name exception
\name:
	pushState
	call storeState
	popState

	mov rdi, \exception 	# Exception code
	mov rsi, [rsp]		# EIP at exception generation
	call exceptionDispatcher
	
	iretq
.endm

/* --------------------------------------------------------------------------------------------------------------------------------------------------
													EXCEPTIONS ATTENTION ROUTINES
-------------------------------------------------------------------------------------------------------------------------------------------------- */


exceptionHandler _exception0Handler 0		# Zero Division Exception

exceptionHandler _exception6Handler 6		# Invalid Opcode Exception
