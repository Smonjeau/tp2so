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

.intel_syntax noprefix

.section .text

_syscallDispatcher:
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

    iretq

_draw:
    call sysDraw
    iretq
    
_getRes:
    call sysGetRes
    iretq

_memDump:
    call sysMemDump
    iretq

_getTime:
    call sysGetTime
    iretq

_cpuInfo:
    call sysCpuInfo
    iretq

_cpuTemp:
    call sysCPUTemp
    iretq

_getRegBkp:
    call sysGetRegBkp
    iretq
