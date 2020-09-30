.extern schedule
.extern createProcessPCB
.extern malloc
.global switchProcessContext
.global switchProcessContextBuenarda
.global createProcessContext
.intel_syntax noprefix

.section .text
.macro popState
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
.endm
 

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





switchProcessContextBuenarda:
	pop retAddress
	add rsp,8
	mov rdi, rsp
	call schedule
    
	mov rsp, rax
	popState 
	push retAddress
	ret

switchProcessContext:
	pop retAddress
	mov rdi, rsp
	call schedule
    mov rsp, rax
	push retAddress
	ret

createProcessContext:
	pop retAddress

	# Reserve 1kb for previous process stack growth
	  sub rsp, 1024
    

    # Interrupt data
    push 0      # SS
    push rbp    # RSP
    push 0x202  # RFLAGS
    push 0x8    # CS
    push rdx 	# RIP	rsp+15*8

    # State data
    push 0      # r15
	push 0      # r14
	push 0      # r13
	push 0      # r12
	push 0      # r11
	push 0      # r10
	push 0      # r9
	push 0      # r8
	push rbp    # rbp
	push rdi    # rdi (argc)
	push rsi    # rsi (argv)
	push rdx    # rdx
	push rcx    # rcx
	push rbx    # rbx
	push rax    # rax

	mov rdi, rsp
	call createProcessPCB

	push retAddress
	ret

.section .data
	retAddress: .skip 8
