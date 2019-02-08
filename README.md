# GFX Adventures

this repo is a collection of how i'm learning graphics, straight into the deep end. i'm a bit of an aspiring technical artist at this point and Unity's ShaderLab has given me almost too much niceties I'm rebuilding here.. probably too much.

i'll probably stick to c++ the whole time. sorta tested cross platform but not really, 100% windows and mac, though.

this project is organized into chapters/branches, with master being the current chapter, synced to it's own branch.  
*this wasn't always the case though, so broken branches might exist.*

## building & running

**Requirements**
- premake5
- *(linux/mac)* - clang 7+ or gcc 8+, make
- *(linux/mac)* - glfw3, glew libs
- *(mac)* - macOS SDK Headers (if they are missing, [read](https://donatstudios.com/MojaveMissingHeaderFiles))
- *(windows)* - visual studio 2017

### Windows 

#### Visual Studio 2017

Run:
```ps1
premake5 vs2017
.\GFXAdventures.sln
```
Click Run/Debug, **Done.**

#### VS2017 Developer Console

```ps1
run.bat
# which is equivalent to
premake5 vs2017
msbuild
.\.bin\main.exe
```

\**for VSCode, you can add this to workspace config to launch Dev Console in the integrated terminal*
```json
    "terminal.integrated.shell.windows": "C:\\Windows\\System32\\cmd.exe",
    "terminal.integrated.shellArgs.windows": [
        "/k", "C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\Common7\\Tools\\VsDevCmd.bat"
    ]
```

Full VSCode tooling is provided as well, please tweak to your liking.

### Mac & Linux 
Run:
```bash
./run.sh
# -- which is equivalent to --
premake5 gmake2
make -j
.bin/main

# -- alternative build configs --
env config=debug ./run.sh        # (the default) a very verbose debugging mode w/ symbols.
env config=debugshaders ./run.sh # to debug shader preprocessor/loader internals as well
env config=release ./run.sh      # for no debugging at all. possibly faster.
```
**Done.**

Full VSCode tooling is provided as well, please tweak to your liking.


## todo

- [x] render 2d stuff!
- [ ] render 3d stuff!
- [ ] engine framework-ish
- [ ] ???
- [ ] physics
- [x] windows support/tooling
- [x] premake5, because i was too lazy to install it
- [ ] fix weird malloc free break on exit.

*stuff for way later*

- [ ] OpenVR
- [ ] direct3d
- [ ] vulkan

## some cool things

- `#include` directive in GLSL
- `#pragma vertex/fragment/etc` from ShaderLab

## chapters/diary

### 01 A Beginning

[branch][01] -- no windows support.

so this was just to get stuff on the screen. i started this on mac, opengl. even with a few quirks, namely that it won't render unless the window is bumped, it worked rather well. i wanted to try getting it running on windows, but c++17 STL and windows still don't really get along, and things didn't compile well for me. eventually rewrote it to no avail, and reverted. but thennn, i swapped to linux on a VM, and things worked out. my previous assumption that VS for C++ is fucking dumb is where my mind stands. even trying mingw, gcc, and clang on windows gave me trash. nothing to do.

editing a shader and pressing page up will reload them. press escape to exit.

### 02 Frameworks

[branch][02] -- no windows support.

so this is where i learned all about smart pointers, classes, and some other really dumb things about C++ that i was poisoned away from in C#/Go. eventually everything made sense, though, thanks to some friends. one of the same friends also introduced me to premake, which is the thing i've enjoyed most about this whole experience. 

i worked quite a bit on the shader system, i think it's the most perfect system i could have devised for myself. even got into preprocessing a little, i might dig deeper into that in the future, I all too much enjoy Unity's ShaderLab/HLSL stuff.

i almost have full feature parity with 01, but i decided to take a detour to windows for 03, as all that's left is some weirdo shader things.

this one also lost me a lot of sleep, accidentally got too far down the pointer rabbit hole.

### 03 In a Window, Darkly.

[branch][03]

02's detour into making this system work on.. windows.. and some other detour-y things.

even though this seems like a simple task, it was kind of was. this was a complete mess of tooling issues, both my on my system and in my code. windows support wasn't difficult, as I already sorta realized what I needed to do for it... but i come back to my mac and find everything scuffed again. and regex, out of all things, had a massive bug. but the bug? it wasn't a bug at all. it was a linker problem. any atom (think `.`), in any form mind you (incl. \x00 and etc), would segfault on search. i spent maybe 2 hours going through things to realize this. turns out, if i compiled with mac's gcc alias (but it was actually clang but apple edition,) it would actually work. so that means my user mode clang's ld.lld was linking in a wild direction and either SIGSEGV or SIGBUS-ing any time that code ran. i removed my own alias and it started working. jfc.

### 04 Actual Graphics

[branch][04]

time for actually like.. doing opengl things. i guess.

made a rudimentary material system. need to improve it.

[01]: https://github.com/kayteh/gfx-adventures/tree/01-a-beginning
[02]: https://github.com/kayteh/gfx-adventures/tree/02-frameworks
[03]: https://github.com/kayteh/gfx-adventures/tree/03-in-a-window-darkly
[04]: https://github.com/kayteh/gfx-adventures/tree/04-actual-graphics