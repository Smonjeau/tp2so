#!/bin/bash
cd /home
make clean -s
make MM=$1 -s