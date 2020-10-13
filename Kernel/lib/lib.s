/*---------------------------------------------------------------------------------------------------
|   LIB.S  |                                                                             			|
|-----------                                                                               			|
| This file provides miscellaneous assembler functions.                                         	|
---------------------------------------------------------------------------------------------------*/

.global _cli
.global _sti
.global _hlt
.global _xchg

.intel_syntax noprefix

.section .text

_hlt:
    hlt
    ret


_cli:
	cli
	ret


_sti:
	sti
	ret

_xchg:
    mov rax, rsi
    xchg [rdi], eax
    ret
