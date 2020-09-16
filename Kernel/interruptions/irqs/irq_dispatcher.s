/*---------------------------------------------------------------------------------------------------
|   IRQ_DISPATCHER.C  |                                                                 			|
|----------------------                                                                   			|
| This file provides the IRQ attention routines, as well as the functions for masking the PIC.    	|
---------------------------------------------------------------------------------------------------*/


/* --------------------------------------------------------------------------------------------------------------------------------------------------
															DECLARATIONS
-------------------------------------------------------------------------------------------------------------------------------------------------- */

.global picMasterMask
.global picSlaveMask

.global _irq00Handler
.global _irq01Handler

.extern irqDispatcher
.extern read
.extern storeState

.intel_syntax noprefix

.section .text

/* --------------------------------------------------------------------------------------------------------------------------------------------------
															MACROS
-------------------------------------------------------------------------------------------------------------------------------------------------- */

.include "./interruptions/macros.s"

.macro irqHandlerMaster name irq
\name:
	pushState

	mov rdi, \irq
	call irqDispatcher

	mov al, 0x20	# Signal pic EOI (End of Interrupt)
	out 0x20, al

	popState
	iretq
.endm


/* --------------------------------------------------------------------------------------------------------------------------------------------------
														PIC MASKS CONTROLLERS
-------------------------------------------------------------------------------------------------------------------------------------------------- */


picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	0x21,al
    pop rbp
    ret


picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  # ax = mascara de 16 bits
    out	0xA1,al
    pop     rbp
    ret


/* --------------------------------------------------------------------------------------------------------------------------------------------------
													INTERRUPTIONS ATTENTION ROUTINES
-------------------------------------------------------------------------------------------------------------------------------------------------- */


irqHandlerMaster _irq00Handler 0		# Timer Tick

_irq01Handler:			# Keyboard
	pushState

	call read			# Before lending control to dispatcher, it checks if TAB key has been pressed
	cmp rax, 15			# If so, it stores the state of the registers, this state can be accessed later through a syscall
	jne _skipStore

	call storeState

	_skipStore: mov rdi, 1
	call irqDispatcher

	mov al, 0x20
	out 0x20, al

	popState
	iretq
