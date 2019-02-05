#!/bin/sh

INCLUDES=

uname -a | grep -iq "linux" && INCLUDES="-lGL -lglfw -lGLEW"
uname -a | grep -iq "darwin" && INCLUDES="-framework OpenGL -lglfw -lglew"

clang++ $INCLUDES -I../includes -std=c++17 -DWINDOW_TITLE="\"GFX Adventures $(basename $PWD)\"" "$@"