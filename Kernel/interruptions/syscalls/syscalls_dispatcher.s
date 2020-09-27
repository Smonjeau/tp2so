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
_malloc:
    call sysMalloc 
    iretq

_free:
    call sysFree
    iretq

_ps:
    call sysPS
    iretq

_memStatus:
    call sysMemStatus 
    iretq

_startProcess:
    call sysStartProcess 
    iretq
