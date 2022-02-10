workspace "Physics-Engine-2D"
    platforms { "Win32" , "Win64" }
    configurations { "Debug" , "Release" }


filter { "platforms:Win32" }
    system "Windows"
    architecture "x86"

filter { "platforms:Win64" }
    system "Windows"
    architecture "x86_64"


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Olala/vendor/imgui"
    include "Olala/vendor/yaml-cpp"
group ""

include "Physics"
include "Olala"
include "Editor"
include "Sandbox"
    