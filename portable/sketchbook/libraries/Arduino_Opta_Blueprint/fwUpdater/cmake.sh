#!/bin/bash

# SCRIPT:      cmake.sh
# AUTHOR:      Daniele Aimo
# DATE:        20240109
# REV:         0.1.A (A, B, D, T and P for Alpha, Beta, Dev, Test and Production)
# PLATFORM:    
# PURPOSE:     lauch cmake to produce build directory


if [ -d ./build ]
then
	rm -R ./build
fi

mkdir ./build

cmake -S . -B ./build


