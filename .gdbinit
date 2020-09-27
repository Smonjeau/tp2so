target remote localhost:1234

add-symbol-file ~/SO/Practicas/TPE2/Kernel/kernel.elf 0x100000
add-symbol-file ~/SO/Practicas/TPE2/Userland/main_app.elf 0x400000

maintenance packet Qqemu.PhyMemMode:1

python
import subprocess
folders=subprocess.check_output(["find Kernel -type d -printf '%p ' && find Userland -type d -printf '%p '"], shell=True).decode("utf-8")
gdb.execute("directory "+folders)
end