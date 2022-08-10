#!bin/sh

# Clean
rm -rf bin
mkdir bin
cd bin

# Copy assets
cp ../assets/ ./ -r
cp ../src/shaders ./assets/ -r

flags=(
	-std=gnu99 -w -O3
)

inc=(
	-I ../third_party/include/
)

libs=(
	-lopengl32
	-lkernel32
	-luser32
	-lshell32
	-lgdi32
	-lWinmm
	-lAdvapi32
	-lm
)

# Build game
proj_name=game
echo Building ${proj_name}...
src=(
	../src/main.c
	../src/**/*.c
)
build_cmd="gcc ${inc[*]} ${src[*]} ${flags[*]} ${libs[*]} -o ${proj_name}"
echo ${build_cmd}
${build_cmd}

# Build model viewer
proj_name=modelviewer
echo Building ${proj_name}...
src=(
	../src/model_viewer.c
	../src/**/*.c
)
build_cmd="gcc ${inc[*]} ${src[*]} ${flags[*]} ${libs[*]} -o ${proj_name}"
echo ${build_cmd}
${build_cmd}
