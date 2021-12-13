workspace "Physics Engine 2D"
    architecture "x86"

    configurations { "Debug" , "Release" }


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


include "Olala"
include "Editor"
include "Sandbox"