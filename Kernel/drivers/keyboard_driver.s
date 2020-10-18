/*---------------------------------------------------------------------------------------------------
|   KEYBOARD_DRIVER.C    |                                                                          |
|------------------------                                                                           |
| This function provides the interface for reading values in the keyboard buffer.         	        |
---------------------------------------------------------------------------------------------------*/


.globl read
.intel_syntax noprefix

.section .text

read:
    mov rax, 0
    in al, 0x60
    ret
