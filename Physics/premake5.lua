project "Physics"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "%{wks.location}/Olala/vendor"
    }

    filter "system:windows"
		systemversion "latest"


	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"


	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"