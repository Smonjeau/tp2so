include Makefile.inc
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SOURCES=$(call rwildcard, ., *.c *.s)

KERNEL=kernel.bin

STATICLIBS=

all: $(KERNEL)		# Default rule, requisite is kernel rule

$(KERNEL): $(SOURCES) $(STATICLIBS)	# All .o's are requisite
	$(GCC) $(GCCFLAGS) -I./include -T kernel.ld -o $(KERNEL) $(SOURCES) $(STATICLIBS)


clean:
	rm -rf */*.o *.bin

.PHONY: all clean
