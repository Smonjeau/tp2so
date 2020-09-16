/*---------------------------------------------------------------------------------------------------
|   CPU_INFO.C    |                                                                                 |
|------------------                                                                                 |
| These functions provide information about the CPU by using CPUID.        	                        |
---------------------------------------------------------------------------------------------------*/


.global getBrandName
.global getBrandIndex
.global getTemperature
.intel_syntax noprefix

.section .text


# --------------------------------------------------
#           getBrandName & getBrandIndex
# --------------------------------------------------
# Brand name returns a string description
# Brand index returns a number that can be matched to a string with more detials of processor

.macro _getBrand part
    mov eax, 0x80000002+\part
    cpuid
    mov [rdi+16*\part+0], eax
    mov [rdi+16*\part+4], ebx
    mov [rdi+16*\part+8], ecx
    mov [rdi+16*\part+12], edx
.endm

getBrandName:
    push rbx

    _getBrand 0
    _getBrand 1
    _getBrand 2
    movb [rsi+49], 0

    pop rbx
    ret


getBrandIndex:
    mov eax, 1
    cpuid
    mov eax, ebx
    and eax, 0xF
    ret


# --------------------------------------
#           getTemperature
# --------------------------------------
# MSR information taken from https://courses.cs.washington.edu/courses/cse451/18sp/readings/ia32-4.pdf, page 27 and 263
# The structures of MSR vary from architecture, Kaby Lake was taken for this function
# IA32_THERMAL_STATUS holds an offset from TCC activation temperature, which is holded by MSR_TEMPERATURE_TARGET
# The values of the registers have been hardcoded for testing purposes. Since QEMU emulates the CPU, it always gives a temperature of zero
# rdmsr -a 418 and rdmsr -a 412 have been used for obtaining the hardcoded values

getTemperature:
    mov rdx, 0

    mov rax, 0x83c2808    # IA32_THERMAL_STATUS (0x19C, hardcoded)

    and rax, 0xDF0000   # Get bytes 16-23
    shr rax, 16
    mov rdi, rax

    mov rax, 0x5640000    # MSR_TEMPERATURE_TARGET (0x1A2, hardcoded)

    and rax, 0xDF0000   # Get bytes 16-23
    shr rax, 16

    sub rax, rdi   # Substract target-offset and return
    ret
