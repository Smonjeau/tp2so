
buddy:  bootloader kernelBuddy userland image 
all:  bootloader kernel userland image

bootloader:
	cd Bootloader; make all

kernelBuddy:
	cd Kernel;make CFLAGS=-Dbuddy=1 all
kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: bootloader image collections kernel userland all clean
