solution "GFXAdventures"
  project "02-Frameworks"
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