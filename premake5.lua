#!lua

workspace "GFXAdventures"
  configurations { "Debug", "Release" }
  includedirs "./includes/**"
  targetname "main"
  cppdialect "C++17"
  
  filter "system:Linux"
  links { "GL", "glfw", "GLEW" }
  
  filter "system:Mac"
  links { "OpenGL.framework", "glfw", "glew" }
  
  filter "system:Linux or Mac"
    toolset "clang"
    -- buildoptions { "-std=c++17" }
    links { "stdc++fs" }

  filter "configurations:Debug"
    buildoptions { "-pedantic" }
    defines { "DEBUG" }
    symbols "On"
    optimize "Off"
    warnings "Extra"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"

  projects = os.matchfiles("./**/premake5.lua")
  for _,path in pairs(projects) do
    dofile (path)
  end

  