workspace "Seacrest"
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
IncludeDir["ImGui"] = "Seacrest/vendor/imgui"
IncludeDir["wic_image"] = "Seacrest/vendor/wic_image"

group "Dependencies"
    include "Seacrest/vendor/imgui"
group ""

project "Seacrest"
    location "Seacrest"
    kind "StaticLib"
    characterset "MBCS"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "scpch.h"
    pchsource "Seacrest/src/scpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/wic_image/**.h",
        "%{prj.name}/vendor/wic_image/**.cpp",
        "%{prj.name}/src/**.hlsl"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.wic_image}"
    }

    links
    {
        "ImGui"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "SEACREST_PLATFORM_WINDOWS",
            "SEACREST_BUILD_DLL"
        }

    filter "configurations:Debug"
        defines "SEACREST_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "SEACREST_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "SEACREST_DIST"
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
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/Resources/**.rc"
    }

    includedirs
    {
        "Seacrest/vendor/spdlog/include",
        "Seacrest/src",
        "Seacrest/vendor"
    }

    links
    {
        "Seacrest"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "SEACREST_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "SEACREST_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "SEACREST_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "SEACREST_DIST"
        runtime "Release"
        optimize "on"
