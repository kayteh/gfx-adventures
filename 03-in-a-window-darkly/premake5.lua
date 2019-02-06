workspace "GFXAdventures"
  project "03-In-A-Window-Darkly"
    kind "ConsoleApp"
    language "C++"
    includedirs "../includes"
    files { "**.h", "**.cpp" }
    targetdir ".bin/"