GCC=gcc
GPP=g++
LD=ld
AR=ar
ASM=nasm

GCCFLAGS=-m64 -fno-exceptions -std=c99 -Wall -ffreestanding -nostdlib -fno-common -mno-red-zone -mno-mmx -fno-builtin-malloc -fno-builtin-free -fno-builtin-realloc -Wno-unused-function -Wno-div-by-zero -Wno-unused-but-set-variable
DEBUGFLAGS=-g -Wl,--oformat=elf64-x86-64 -fdebug-prefix-map=..=$(readlink -f ..)
ARFLAGS=rvs
ASMFLAGS=-felf64
