/*---------------------------------------------------------------------------------------------------
|   SYSCALLS_DISPATCHER.S    |                                                          			|
|-------------------------------                                                          			|
| This file provides exception handling interruption 0x80 attention routine.		                |
---------------------------------------------------------------------------------------------------*/

.globl _syscallDispatcher

.extern sysRead
.extern sysDraw
.extern sysGetRes
.extern sysMemDump
.extern sysGetTime
.extern sysCpuInfo
.extern sysCPUTemp
.extern sysGetRegBkp
.extern sysMalloc 
.extern sysMemStatus 
.extern sysFree
.extern sysPS
.extern sysStartProcess
.extern sysKill
.extern sysGetPid
.extern sysBlock
.intel_syntax noprefix

.section .text

.include "./interruptions/macros.s"


_syscallDispatcher:
    pushStateNoRax

    cmp rax, 0
    je _read

    cmp rax, 1
    je _draw

    cmp rax, 2
    je _getRes

    cmp rax, 3
    je _memDump

    cmp rax, 4
    je _getTime

    cmp rax, 5
    je _cpuInfo

    cmp rax, 6
    je _cpuTemp

    cmp rax, 7
    je _getRegBkp

    cmp rax, 8
    je _malloc

    cmp rax,9
    je _free 

    cmp rax,10 
    je _memStatus

    cmp rax,11
    je _startProcess

    cmp rax, 12
    je _ps

    cmp rax,13
    je _kill

    cmp rax,14
    je _get_pid

    cmp rax, 15
    je _block

    jmp endOfInt



endOfInt:
	push rax
    mov al, 0x20
	out 0x20, al
    pop rax
	popStateNoRax
	iretq

_read:
    call sysRead
    jmp endOfInt

_draw:
    call sysDraw
    jmp endOfInt
    
_getRes:
    call sysGetRes
    jmp endOfInt

_memDump:
    call sysMemDump
    jmp endOfInt

_getTime:
    call sysGetTime
    jmp endOfInt

_cpuInfo:
    call sysCpuInfo
    jmp endOfInt

_cpuTemp:
    call sysCPUTemp
    jmp endOfInt

_getRegBkp:
    call sysGetRegBkp
    jmp endOfInt
_malloc:
    call sysMalloc 
    jmp endOfInt

_free:
    call sysFree
    jmp endOfInt

_ps:
    call sysPS
    jmp endOfInt

_memStatus:
    call sysMemStatus 
    jmp endOfInt

_startProcess:
    call sysStartProcess 
    jmp endOfInt

_kill:
    call sysKill
    jmp endOfInt

_get_pid:
    call sysGetPid
    jmp endOfInt

_block:
    call sysBlock
    jmp endOfInt

    