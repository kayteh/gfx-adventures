#!lua

workspace "GFXAdventures"
  configurations { "Debug", "Release", "DebugShaders" }
  includedirs { "./includes" }
  targetname "main"
  cppdialect "C++17"
  
  -- UNIXY STUFF
  filter "system:Linux or MacOSX"
    toolset "clang"
    buildoptions { "-ffast-math" }

  filter "system:Linux"
    links { "GL", "glfw", "GLEW", "stdc++fs" }
    defines { "__LINUX__" }
  
  filter "system:MacOSX"
    links { "OpenGL.framework", "glfw", "glew", "c++fs" }
    libdirs { "/usr/local/opt/llvm/lib" }
    defines { "__APPLE__" }
  
  -- WINDOWSY STUFF
  filter "system:Windows"
    libdirs { "./vendor/win32" }
    links { "OpenGL32", "glfw3", "glew32s" }
    defines { "__WIN32__" }
    buildoptions { "/fp:fast" }
    
  -- RELEASE STUFF
  filter "configurations:Debug or DebugShaders"
    defines { "DEBUG" }
    symbols "On"
    optimize "Debug"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"

  filter "configurations:DebugShaders"
    defines { "DEBUG_SHADERS" }

  project "GFXAdventures"
    kind "ConsoleApp"
    language "C++"
    includedirs "./includes"
    files { "**.h", "**.cpp" }
    targetdir ".bin/"

  