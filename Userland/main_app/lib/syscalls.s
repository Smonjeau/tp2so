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

syscall draw, 1
syscall getRes, 2
syscall memDump, 3
syscall getTime, 4
syscall cpuInfo, 5
syscall cpuTemp, 6
syscall getRegBkp, 7
