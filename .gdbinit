target remote localhost:1234
add-symbol-file Kernel/kernel.elf 0x100000
add-symbol-file Userland/main_app.elf 0x400000