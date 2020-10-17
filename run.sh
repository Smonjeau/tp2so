#!/bin/bash
if [ "$1" = "buddy" ]
then
    docker run -v `pwd`:/home --security-opt seccomp:unconfined -ti agodio/itba-so:1.0 /home/compile.sh BUDDY
else
    docker run -v `pwd`:/home --security-opt seccomp:unconfined -ti agodio/itba-so:1.0 /home/compile.sh BITMAP
fi

if [ "$1" = "gdb" -o "$2" = "gdb" ]
then
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 1024 -s -S
else
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 1024
fi

make clean -s
