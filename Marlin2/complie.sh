#!/bin/bash

mkdir -p ./bin

if [ "$1" = "gcc" ]; then
    echo "gcc complie!"
	make all -j -f Makefile_gcc
elif [ "$1" = "clean" ]; then
    echo "gcc clean!"
	make clean -f Makefile_gcc
else
    echo "gcc complie!"
	make all -j -f Makefile_gcc
fi

