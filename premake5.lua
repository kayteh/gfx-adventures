#!lua

workspace "GFXAdventures"
  configurations { "Release", "Debug" }
  includedirs "./includes/**"
  targetname "main"

  filter "linux"
    links { "GL", "glfw", "GLEW" }
  
  filter "macosx"
    links { "OpenGL.framework", "glfw", "glew" }

  filter "linux or macosx"
    toolset "clang"
    buildoptions { "-std=c++17", "-lstdc++fs" }

  filter "Debug"
    buildoptions { "-Wall", "-Wextra", "-pedantic" }

  filter "Debug"
    defines { "DEBUG" }
    symbols "On"
    optimize "Off"


  filter "Release"
    defines { "NDEBUG" }
    optimize "On"

  projects = os.matchfiles("./**/premake5.lua")
  for _,path in pairs(projects) do
    dofile (path)
  end

  