/*---------------------------------------------------------------------------------------------------
|   ASMLIB.S    |                                                                                   |
|----------------                                                                                   |
| This library provides some low level functions that are used among the program                    |
---------------------------------------------------------------------------------------------------*/

.global regDump
.global halt
.global sti
.intel_syntax noprefix

.section .text

regDump:
    mov [rdi], rax
    mov [rdi+8], rbx
    mov [rdi+16], rcx
    mov [rdi+24], rdx
    mov [rdi+32], rsi
    mov [rdi+40], rdi

    mov [rdi+48], rbp

    add rsp, 8      # Load RSP prev to function call (only RIP has been pushed)
    mov [rdi+56], rsp
    sub rsp, 8

    mov rax, [rsp]      # Load RIP prev to function call
    mov [rdi+64], rax

    mov [rdi+72], r8
    mov [rdi+80], r9
    mov [rdi+88], r10
    mov [rdi+96], r11
    mov [rdi+104], r12
    mov [rdi+112], r13
    mov [rdi+120], r14
    mov [rdi+128], r15
    ret

halt:
    hlt
    ret

sti:
    sti
    ret
