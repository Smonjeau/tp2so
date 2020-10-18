/*---------------------------------------------------------------------------------------------------
|   EXCEPTIONS_DISPATCHER.S    |                                                          			|
|-------------------------------                                                          			|
| This file provides exception handling routines.											    	|
---------------------------------------------------------------------------------------------------*/

.global _exception0Handler
.global _exception1Handler
.global _exception2Handler
.global _exception3Handler
.global _exception4Handler
.global _exception5Handler
.global _exception6Handler
.global _exception7Handler
.global _exception8Handler
.global _exception9Handler
.global _exception10Handler
.global _exception11Handler
.global _exception12Handler
.global _exception13Handler
.global _exception14Handler
.global _exception15Handler
.global _exception16Handler
.global _exception17Handler
.global _exception18Handler
.global _exception19Handler
.global _exception20Handler
.global _exception21Handler
.global _exception22Handler
.global _exception23Handler
.global _exception24Handler
.global _exception25Handler
.global _exception26Handler
.global _exception27Handler
.global _exception28Handler
.global _exception29Handler
.global _exception30Handler
.global _exception31Handler

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
	mov rsi, [rsp+8]		# EIP at exception generation
	call exceptionDispatcher
	
	iretq
.endm

/* --------------------------------------------------------------------------------------------------------------------------------------------------
													EXCEPTIONS ATTENTION ROUTINES
-------------------------------------------------------------------------------------------------------------------------------------------------- */


exceptionHandler _exception0Handler 0		# Zero Division Exception
exceptionHandler _exception1Handler 1
exceptionHandler _exception2Handler 2
exceptionHandler _exception3Handler 3
exceptionHandler _exception4Handler 4
exceptionHandler _exception5Handler 5
exceptionHandler _exception6Handler 6		# Invalid Opcode Exception
exceptionHandler _exception7Handler 7
exceptionHandler _exception8Handler 8
exceptionHandler _exception9Handler 9
exceptionHandler _exception10Handler 10
exceptionHandler _exception11Handler 11
exceptionHandler _exception12Handler 12
exceptionHandler _exception13Handler 13		# General Fault Protection
exceptionHandler _exception14Handler 14
exceptionHandler _exception15Handler 15
exceptionHandler _exception16Handler 16
exceptionHandler _exception17Handler 17
exceptionHandler _exception18Handler 18
exceptionHandler _exception19Handler 19
exceptionHandler _exception20Handler 20
exceptionHandler _exception21Handler 21
exceptionHandler _exception22Handler 22
exceptionHandler _exception23Handler 23
exceptionHandler _exception24Handler 24
exceptionHandler _exception25Handler 25
exceptionHandler _exception26Handler 26
exceptionHandler _exception27Handler 27
exceptionHandler _exception28Handler 28
exceptionHandler _exception29Handler 29
exceptionHandler _exception30Handler 30
exceptionHandler _exception31Handler 31
