workspace "Physics-Engine-2D"
    architecture "x86"

    configurations { "Debug" , "Release" }


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Olala/vendor/imgui"
    include "Olala/vendor/yaml-cpp"
group ""

include "Physics"
include "Olala"
include "Editor"
include "Sandbox"