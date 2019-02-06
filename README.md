# GFX Adventures

this repo is a collection of how i'm learning graphics, straight into the deep end.

probably only c++, probably only works on linux/mac for now. d3d is a goal but not a force.

## building & running

to build, you need premake5, clang 7+ (ideally,) and gnu make

```bash
# generate the project (only need to run once)
premake5 gmake

# build all
make

# or a specific one,
cd 01-a-beginning
make

# then, run
./main
```

or use VS2017
```
premake5 vs2017
```

then open the solution.

## todo

- [x] render 2d stuff!
- [ ] engine framework-ish
- [ ] ???
- [ ] render 3d stuff!
- [ ] physics
- [x] windows support/tooling
- [x] premake5, because i was too lazy to install it

*stuff for way later*

- [ ] OpenVR
- [ ] direct3d
- [ ] vulkan

## chapters/diary

### 01 A Beginning

so this was just to get stuff on the screen. i started this on mac, opengl. even with a few quirks, namely that it won't render unless the window is bumped, it worked rather well. i wanted to try getting it running on windows, but c++17 STL and windows still don't really get along, and things didn't compile well for me. eventually rewrote it to no avail, and reverted. but thennn, i swapped to linux on a VM, and things worked out. my previous assumption that VS for C++ is fucking dumb is where my mind stands. even trying mingw, gcc, and clang on windows gave me trash. nothing to do.

editing a shader and pressing page up will reload them. press escape to exit.

### 02 Frameworks

so this is where i learned all about smart pointers, classes, and some other really dumb things about C++ that i was poisoned away from in C#/Go. eventually everything made sense, though, thanks to some friends. one of the same friends also introduced me to premake, which is the thing i've enjoyed most about this whole experience. 

i worked quite a bit on the shader system, i think it's the most perfect system i could have devised for myself. even got into preprocessing a little, i might dig deeper into that in the future, I all too much enjoy Unity's ShaderLab/HLSL stuff.

i almost have full feature parity with 01, but i decided to take a detour to windows for 03, as all that's left is some weirdo shader things.

this one also lost me a lot of sleep, accidentally got too far down the pointer rabbit hole.

### 03 In a Window, Darkly.

02's detour into making this system work on.. windows.. and some other detour-y things.

even though this seems like a simple task, it was kind of was. this was a complete mess of tooling issues, both my on my system and in my code. windows support wasn't difficult, as I already sorta realized what I needed to do for it... but i come back to my mac and find everything scuffed again. and regex, out of all things, had a massive bug. but the bug? it wasn't a bug at all. it was a linker problem. any atom (think `.`), in any form mind you (incl. \x00 and etc), would segfault on search. i spent maybe 2 hours going through things to realize this. turns out, if i compiled with mac's gcc alias (but it was actually clang but apple edition,) it would actually work. so that means my user mode clang's ld.lld was linking in a wild direction and either SIGSEGV or SIGBUS-ing any time that code ran. i removed my own alias and it started working. jfc.