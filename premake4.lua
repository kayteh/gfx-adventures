#!lua

-- A solution contains projects, and defines the available configurations
solution "GFXAdventures"
  configurations { "Release", "Debug" }
  includedirs "./includes/**"
  targetname "main"

  configuration "linux"
    links { "GL", "glfw", "GLEW", "glm" }
  
  configuration "macosx"
    links { "OpenGL.framework", "glfw", "glew" }

  configuration "linux or macosx"
    buildoptions { "-std=c++17" }

  projects = os.matchfiles("./**/premake4.lua")
  for _,path in pairs(projects) do
    dofile (path)
  end

  