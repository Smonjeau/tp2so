.extern schedule
.extern createProcessPCB
.extern createProcessPCBFromKernel
.extern malloc
.global switchProcessContext
.global createProcessContext
.intel_syntax noprefix

.section .text

.include "./interruptions/macros.s"

/*
This is a test function, it sets the specified process context, skipping the scheduler
.global setProcess
setProcess:
	mov rsp, rdi
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop rbp
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
	iretq
*/

switchProcessContext:
	pop retAddress
	mov rdi, rsp
	call schedule
    mov rsp, rax
	push retAddress
	ret


createProcessContext:
	cli

	# Preserve calling args
	push rdi
	push rsi
	push rdx
	push rcx

	# Reserve 4kb for process stack
	mov rdi, 2048
	call malloc
	cmp rax,0
	je no_space

	# Restore calling args
	pop rcx
	pop rdx
	pop rsi
	pop rdi

	# Ante dudas sobre por qué restamos 4088 consultar
	
	# Calc the base of the stack
	add rax, 4096
	sub rax, 8

	# The substracion of 8 bytes is because the last byte allowed to be written is malloc+4096
	# so the RBP needs to point to the previous 8 bytes (stack word size)

    # Interrupt data
    movq [rax-8*0], 0     	# SS
    movq [rax-8*1], rax   	# RSP
    movq [rax-8*2], 0x202  	# RFLAGS
    movq [rax-8*3], 0x8    	# CS
    movq [rax-8*4], rdx 	# RIP	rsp+15*8

    # State data
    movq [rax-8*5], 1      # r15
	movq [rax-8*6], 2    	# r14
	movq [rax-8*7], 3    	# r13
	movq [rax-8*8], 4    	# r12
	movq [rax-8*9], 5    	# r11
	movq [rax-8*10], 6    	# r10
	movq [rax-8*11], 7   	# r9
	movq [rax-8*12], 8     	# r8
	movq [rax-8*13], rax   	# rbp
	movq [rax-8*14], rdi   	# rdi (argc)
	movq [rax-8*15], rsi   	# rsi (argv)
	movq [rax-8*16], 3    	# rdx
	movq [rax-8*17], 2    	# rcx
	movq [rax-8*18], 1    	# rbx
	movq [rax-8*19], 0    	# rax

	# RAX = segmentAddress
	# RAX-8*19 = contextRSP

	# Calc the segment address for future free
	mov rsi, rax
	sub rsi, 4096

    # The freaking padding
	add rsi, 8

	# Calc the contextRSP
	mov rdi, rax
	sub rdi, 8*19

	# Name of the process 
	mov rdx,rcx

	# Create the process PCB
	call createProcessPCB

	sti
	ret


no_space:
	ret

.section .data
	retAddress: .skip 8
	firstArg: .skip 8
