#!/bin/sh

ctime -begin kiz.ctm
pushd ../bin

gcc ../src/main.c -std=c11 -o kiz -lGL -lGLEW `pkg-config --cflags --static --libs glfw3` -L../src/lua -llua -ggdb -DLOG_USE_COLOR

popd
ctime -end kiz.ctm $LastError
