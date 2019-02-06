workspace "GFXAdventures"
  project "04-Actual-Graphics"
    kind "ConsoleApp"
    language "C++"
    includedirs "../includes"
    files { "**.h", "**.cpp" }
    targetdir ".bin/"