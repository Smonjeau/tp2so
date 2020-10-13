/*---------------------------------------------------------------------------------------------------
|   SYSCALLS.S    |                                                                                 |
|----------------                                                                                   |
| This libary implements callers for all the syscalls provided by the kernel                        |
---------------------------------------------------------------------------------------------------*/

.intel_syntax noprefix
.global valueSet

.macro syscall name, code
.global \name
\name:
    mov eax, \code
    int 0x80
    ret
.endm

syscall read, 0
syscall write, 1

syscall draw, 2
syscall getRes, 3
syscall getTime, 4

syscall cpuInfo, 5
syscall cpuTemp, 6
syscall getRegBkp, 7

syscall memDump, 8
syscall memStatus, 9
syscall malloc, 10
syscall free, 11

syscall startProcess, 12
syscall getPid, 13
syscall ps, 14
syscall block, 15
syscall nice, 16
syscall kill, 17

syscall createSem, 18
syscall waitSem, 19
syscall postSem, 20
syscall deleteSem, 21
