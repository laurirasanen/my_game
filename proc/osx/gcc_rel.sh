#!/bin/bash

rm -rf bin
mkdir bin
cd bin

proj_name=Game
proj_root_dir=$(pwd)/../

flags=(
	-std=c99 -x objective-c -O3 -w 
)

# Include directories
inc=(
	-I ../third_party/include/
)

# Source files
src=(
	../src/main.c
	../src/**/*.c
)

fworks=(
	-framework OpenGL
	-framework CoreFoundation 
	-framework CoreVideo 
	-framework IOKit 
	-framework Cocoa 
	-framework Carbon
)

# Build
echo gcc ${flags[*]} ${fworks[*]} ${inc[*]} ${src[*]} -o ${proj_name}
gcc ${flags[*]} ${fworks[*]} ${inc[*]} ${src[*]} -o ${proj_name}

cd ..



