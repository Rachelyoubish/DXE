workspace "DXE"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["ImGui"] = "DXE/vendor/imgui"

group "Dependencies"
    include "DXE/vendor/imgui"
group ""

project "DXE"
    location "DXE"
    kind "StaticLib"
    characterset "MBCS"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "dxepch.h"
    pchsource "DXE/src/dxepch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.ImGui}"
    }

    links
    {
        "ImGui",
        "d3d11.lib",
        "dxgi.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "DXE_PLATFORM_WINDOWS",
            "DXE_BUILD_DLL"
        }

    filter "configurations:Debug"
        defines "DXE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "DXE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DXE_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        "DXE/src",
        "DXE/vendor"
    }

    links
    {
        "DXE"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "DXE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "DXE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "DXE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "DXE_DIST"
        runtime "Release"
        optimize "on"
