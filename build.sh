#!/bin/sh
clang++ -framework OpenGL -lGLFW -lglew -I../includes -std=c++17 "$@"