.extern schedule
.extern createProcessPCB
.extern createProcessPCBFromKernel
.extern malloc
.global switchProcessContext
.global createProcessContext
.global createProcessContextFromKernel
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

	# Reserve 1kb for process stack
	mov rdi, 1024
	call malloc

	# Restore calling args
	pop rcx
	pop rdx
	pop rsi
	pop rdi

	# Calc the base of the stack
	add rax, 1024

    # Interrupt data
    movq [rax-8*0], 0     	# SS
    movq [rax-8*1], rax    	# RSP
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

	
	# Calc base of the stack, for future free
	sub rax,1024
	mov rsi, rax
	add rax,1024
	# Calc the top of the stack
	sub rax, 8*19
	
	# Create the process PCB
	mov rdi, rax

	# Name of the process 
	mov rdx,rcx

	call createProcessPCB


	sti
	ret



createProcessContextFromKernel:
	

	# Preserve calling args
	push rdi
	push rsi
	push rdx

	# Reserve 1kb for process stack
	mov rdi, 1024
	call malloc

	# Restore calling args
	pop rdx
	pop rsi
	pop rdi

	# Calc the base of the stack
	add rax, 1024

    # Interrupt data
    movq [rax-8*0], 0     	# SS
    movq [rax-8*1], rax    	# RSP
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

	# Calc the top of the stack
	sub rax, 8*19
	
	# Create the process PCB
	mov rdi, rax
	call createProcessPCBFromKernel

	ret

.section .data
	retAddress: .skip 8
	firstArg: .skip 8
