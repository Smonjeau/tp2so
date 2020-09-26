.extern schedule#
.global switchProcess
.global createContext
.intel_syntax noprefix#

.section .text

switchProcess:
	mov rsi, rsp
	call schedule
    mov rsp, rax
	ret

createContext:
    # Interrupt data
    push 0      # SS
    push rbp    # RSP
    push 0x202  # RFLAGS
    push 0x8    # CS
    push rdx 	# RIP

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

	mov rax, rsp
	ret
