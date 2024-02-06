workspace "DeeDeeEngine"
    architecture "x64"
    configurations {
    "Debug",
    "Release",
    "Dist"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir = {}
IncludeDir["GLFW"] = "DeeDeeEngine/vendor/GLFW/include"

include "DeeDeeEngine/vendor/GLFW"

project "DeeDeeEngine"
    location "DeeDeeEngine"
    kind "SharedLib"
    language "C++"

targetdir("bin/" .. outputdir .. "/%{prj.name}")
objdir("bin-int/" .. outputdir .. "/%{prj.name}")
pchheader "deepch.h"
pchsource "DeeDeeEngine/src/deepch.cpp"

files {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
}

includedirs
{
    "DeeDeeEngine/vendor/spdlog/include",
    "DeeDeeEngine/src",
    "%{IncludeDir.GLFW}"
}

links
{
    "GLFW",
    "opengl32.lib"
}

filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    defines {
    "DEE_PLATFORM_WINDOWS",
    "DEE_BUILD_DLL",
}

postbuildcommands{
    ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
}

filter "configurations:Debug"
defines "DEE_DEBUG"
buildoptions "/MDd"
symbols "On"

filter "configurations:Release"
defines "DEE_RELEASE"
buildoptions "/MD"
optimize "On"

filter "configurations:Dist"
defines "DEE_DIST"
buildoptions "/MD"
optimize "On"

project "Sandbox"
location "Sandbox"
kind "ConsoleApp"
language "C++"

targetdir("bin/" .. outputdir .. "/%{prj.name}")
objdir("bin-int/" .. outputdir .. "/%{prj.name}")

files {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
}

includedirs
{
    "DeeDeeEngine/vendor/spdlog/include",
    "DeeDeeEngine/src"
}
links{
    "DeeDeeEngine"
}

filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    defines {
    "DEE_PLATFORM_WINDOWS",
}


filter "configurations:Debug"
defines "DEE_DEBUG"
buildoptions "/MDd"
symbols "On"

filter "configurations:Release"
defines "DEE_RELEASE"
buildoptions "/MD"
optimize "On"

filter "configurations:Dist"
defines "DEE_DIST"
buildoptions "/MD"
optimize "On"