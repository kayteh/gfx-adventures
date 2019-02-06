#!lua

workspace "GFXAdventures"
  configurations { "Debug", "Release" }
  includedirs { "./includes" }
  targetname "main"
  cppdialect "C++17"
  
  filter "system:Linux"
    links { "GL", "glfw", "GLEW", "stdc++fs" }
  
  filter "system:MacOSX"
    links { "OpenGL.framework", "glfw", "glew", "c++fs" }
    libdirs { "/usr/local/opt/llvm/lib" }
  
  filter "system:Linux or MacOSX"
    toolset "clang"
    -- buildoptions { "-std=c++17" }

  filter "system:Windows"
	  libdirs { "./vendor/win32" }
    links { "OpenGL32", "glfw3", "glew32s" }
    defines { "__WIN32__" }

  filter "configurations:Debug"
    buildoptions { "-pedantic" }
    defines { "DEBUG" }
    symbols "On"
    optimize "Debug"
    warnings "Extra"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"

  projects = os.matchfiles("./**/premake5.lua")
  for _,path in pairs(projects) do
    dofile (path)
  end

  