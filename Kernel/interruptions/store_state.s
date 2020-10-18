/*---------------------------------------------------------------------------------------------------
|   STORE_STATE.S   |                                                                               |
|--------------------                                                                               |
| This file has the functions that handle the storage and recover of registers state. The state  	|
| is backed up whenever either TAB is pressed, or an exception is produced.                         |
---------------------------------------------------------------------------------------------------*/

.global storeState
.global dumpState
.intel_syntax noprefix

.section .text

/*
Stack at this point:

irqHandlerMaster EIP
rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15
*/

.macro backupByte i
    mov rax, [rsp+\i]
    mov [backup+\i-8], rax
.endm

.macro dumpByte i, dest
    mov rax, [backup+\i]
    mov [rdi+\i], rax
.endm


storeState:
    backupByte 8    # RAX
    backupByte 16    # RBX
    backupByte 24    # RCX
    backupByte 32
    backupByte 40
    backupByte 48
    backupByte 56
    backupByte 64
    backupByte 72
    backupByte 80
    backupByte 88
    backupByte 96
    backupByte 104
    backupByte 112
    backupByte 120
    backupByte 128
    ret

dumpState:
    dumpByte 0 
    dumpByte 8
    dumpByte 16
    dumpByte 24
    dumpByte 32
    dumpByte 40
    dumpByte 48
    dumpByte 56
    dumpByte 64
    dumpByte 72
    dumpByte 80
    dumpByte 88
    dumpByte 96
    dumpByte 104
    dumpByte 112
    dumpByte 120
    ret


.section .bss

backup: .space 136
