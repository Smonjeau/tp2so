include ../Makefile.inc
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SOURCES=$(call rwildcard, ., *.c *.s)

MODULE_ELF=main_app.elf
MODULE=main_app.bin

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -I./include -T main_app.ld $(SOURCES) -o ../$(MODULE)
	$(GCC) $(GCCFLAGS) -I./include -T main_app.ld $(DEBUGFLAGS) $(SOURCES) -o ../$(MODULE_ELF)

.PHONY: all clean print
