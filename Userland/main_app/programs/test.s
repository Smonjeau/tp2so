.global regDumpTestSet
.global invalidOpcode
.intel_syntax noprefix

.section .text

regDumpTestSet:
    mov rax, 1
    mov rbx, 2
    mov rcx, 3
    mov rdx, 4
    ret


.section .data

invalidOpcode: .byte 6
