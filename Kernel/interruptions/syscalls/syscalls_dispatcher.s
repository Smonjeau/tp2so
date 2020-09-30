/*---------------------------------------------------------------------------------------------------
|   SYSCALLS_DISPATCHER.S    |                                                          			|
|-------------------------------                                                          			|
| This file provides exception handling interruption 0x80 attention routine.		                |
---------------------------------------------------------------------------------------------------*/

.globl _syscallDispatcher

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
.intel_syntax noprefix

.section .text

.include "./interruptions/macros.s"


_syscallDispatcher:
    pushStateNoRax
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

    jmp endOfInt



endOfInt:
	push rax
    mov al,0x20
	out 0x20,al
    pop rax
	popStateNoRax
	iretq

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
    iretq

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
