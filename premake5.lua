workspace "DXE"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "DXE"
	location "DXE"
	kind "SharedLib"
	language "C++"
	characterset "MBCS"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "dxepch.h"
	pchsource "DXE/src/dxepch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"DXE_PLATFORM_WINDOWS",
			"DXE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "DXE_DEBUG"
		buildoptions "/MDd"
		symbols "On"

		filter "configurations:Release"
		defines "DXE_RELEASE"
		buildoptions "/MD"
		optimize "On"

		filter "configurations:Dist"
		defines "DXE_DIST"
		buildoptions "/MD"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"DXE/vendor/spdlog/include",
		"DXE/src"
	}

	links
	{
		"DXE"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"DXE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "DXE_DEBUG"
		buildoptions "/MDd"
		symbols "On"

		filter "configurations:Release"
		defines "DXE_RELEASE"
		buildoptions "/MD"
		optimize "On"

		filter "configurations:Dist"
		defines "DXE_DIST"
		buildoptions "/MD"
		optimize "On"