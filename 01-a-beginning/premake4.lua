solution "GFXAdventures"
  project "01-A-Beginning"
    kind "ConsoleApp"
    language "C++"
    includedirs "../includes"    

    files { "**.h", "**.cpp" }

    configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols" }

    configuration "Release"
      defines { "NDEBUG" }
      flags { "Optimize" }