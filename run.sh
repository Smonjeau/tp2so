#!/bin/bash
docker run -v `pwd`:/home --security-opt seccomp:unconfined -ti agodio/itba-so:1.0 /home/compile.sh

if [[ "$1" = "gdb" ]]; then
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -s -S -d int
else
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512
fi

make clean -s