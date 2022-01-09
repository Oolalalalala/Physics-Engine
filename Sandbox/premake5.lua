project "Sandbox"
    kind "ConsoleApp"
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
		"%{wks.location}/Physics/src",
		"%{wks.location}/Olala/src",
		"%{wks.location}/Olala/vendor",
        "%{wks.location}/Dependencies/GLFW/include",
        "%{wks.location}/Dependencies/GLEW/include"
	}

	links
	{
		"Olala"
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