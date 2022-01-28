project "Olala"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/glm/**.hpp",
        "vendor/glm/**.inl",
        "vendor/spdlog/**.h",
        "vendor/entt/entt.hpp",
        "vendor/stb_image/**.h",
        "vendor/stb_image/**.cpp",
        "vendor/portable_file_dialogs/**.h"
    }

    defines
    {
        "GLEW_STATIC"
    }

    includedirs
    {
        "src",
        "vendor",
        "vendor/imgui",
        "vendor/yaml-cpp/include",
        "%{wks.location}/Physics/src",
        "%{wks.location}/Dependencies/GLFW/include",
        "%{wks.location}/Dependencies/GLEW/include"
    }

    libdirs
    {
        "%{wks.location}/Dependencies/GLFW/lib-vc2019",
        "%{wks.location}/Dependencies/GLEW/lib/Release/Win32"
    }

    links
    {
        "Physics",
        "imgui",
        "yaml-cpp",
        "glfw3.lib",
        "glew32s.lib",
        "opengl32.lib"
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