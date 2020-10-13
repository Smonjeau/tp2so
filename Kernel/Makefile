include Makefile.inc
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SOURCES=$(call rwildcard, ., *.c *.s)

KERNEL_ELF=kernel.elf
KERNEL_SYM=kernel.sym
KERNEL=kernel.bin

STATICLIBS=

all: $(KERNEL)		# Default rule, requisite is kernel rule

$(KERNEL): $(SOURCES) $(STATICLIBS)	# All .o's are requisite
	$(GCC) $(GCCFLAGS) -DMM_$(MM) -I./include -T kernel.ld -o $(KERNEL) $(SOURCES) $(STATICLIBS)
	$(GCC) $(GCCFLAGS) -DMM_$(MM) -I./include -T kernel.ld $(DEBUGFLAGS) -o $(KERNEL_ELF) $(SOURCES) $(STATICLIBS)

	
clean:
	rm -rf */*.o *.elf *.bin

.PHONY: all clean
