#!/bin/bash
docker run -v `pwd`:/home --security-opt seccomp:unconfined -ti agodio/itba-so:1.0 /home/compile.sh
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512
make clean -s