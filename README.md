# GFX Adventures

this repo is a collection of how i'm learning graphics, straight into the deep end.

probably only c++, probably only works on linux/mac for now. d3d is a goal but not a force.

## building & running

to build, you need premake4 and gnu make

```bash
# generate the project (only need to run once)
premake4 gmake

# build all
make

# or a specific one,
cd 01-a-beginning
make

# then, run
./main
```

## todo

- [x] render 2d stuff!
- [ ] engine framework-ish
- [ ] ???
- [ ] render 3d stuff!
- [ ] physics
- [ ] windows support/tooling
- [ ] premake5, because i was too lazy to install it

*stuff for way later*

- [ ] OpenVR
- [ ] direct3d
- [ ] vulkan

## chapters/diary

### 01 A Beginning

so this was just to get stuff on the screen. i started this on mac, opengl. even with a few quirks, namely that it won't render unless the window is bumped, it worked rather well. i wanted to try getting it running on windows, but c++17 STL and windows still don't really get along, and things didn't compile well for me. eventually rewrote it to no avail, and reverted. but thennn, i swapped to linux on a VM, and things worked out. my previous assumption that VS for C++ is fucking dumb is where my mind stands. even trying mingw, gcc, and clang on windows gave me trash. nothing to do.

editing a shader and pressing page up will reload them. press escape to exit.

### 02 Frameworks

so now that i have legs, i may as well stretch my c++ knowledge a bit. probably going to start with makefiles (CMake or premake? idk. something, i know how painful makefiles are from experience.)

bumped opengl version to 4.1, this may make my linux VM salty but i have my mac for now. nbd. seemed to work, so continuing!

