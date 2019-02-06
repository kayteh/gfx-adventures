workspace "GFXAdventures"
  project "01-A-Beginning"
    kind "ConsoleApp"
    language "C++"
    includedirs "../includes"    
    files { "**.h", "**.cpp" }
    targetdir ".bin/"
