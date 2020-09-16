/*---------------------------------------------------------------------------------------------------
|   RTC_DRIVER.C    |                                                                               |
|--------------------                                                                               |
| These functions provide an interface to get information of the Real Time Clock.          	        |
---------------------------------------------------------------------------------------------------*/

.global getSeconds
.global getMinutes
.global getHours
.intel_syntax noprefix

.section .text

.macro get elem
    mov al, \elem
    out 0x70, al
    mov al, 0
    in al, 0x71
    movzx rcx, al
    call bcdToDec
    ret
.endm

getSeconds:
    get 0

getMinutes:
    get 2

getHours:
    get 4

bcdToDec:
    mov rax, 0

    mov bh, cl      # Get higher part
    and bh, 0b11110000
    shr bh, 4

    mov bl, cl      # Get lower part
    and bl, 0b00001111

    mov al, bh      # Multiply higher part by 10
    mov dl, 10
    mul dl

    movzx bx, bl    # Add lower part
    add ax, bx
    
    ret
