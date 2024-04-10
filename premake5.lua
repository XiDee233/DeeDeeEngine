workspace "DeeDeeEngine"
    architecture "x64"
    startproject "Sandbox"
    configurations {
    "Debug",
    "Release",
    "Dist"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir = {}
IncludeDir["GLFW"] = "DeeDeeEngine/vendor/GLFW/include"
IncludeDir["GLAD"] = "DeeDeeEngine/vendor/GLAD/include"
IncludeDir["ImGui"] = "DeeDeeEngine/vendor/imgui"
IncludeDir["glm"] = "DeeDeeEngine/vendor/glm"
IncludeDir["stb_image"] = "DeeDeeEngine/vendor/stb_image"

include "DeeDeeEngine/vendor/GLFW"
include "DeeDeeEngine/vendor/GLAD"
include "DeeDeeEngine/vendor/ImGui"

project "DeeDeeEngine"
    location "DeeDeeEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

targetdir("bin/" .. outputdir .. "/%{prj.name}")
objdir("bin-int/" .. outputdir .. "/%{prj.name}")
pchheader "deepch.h"
pchsource "DeeDeeEngine/src/deepch.cpp"

files {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/vendor/stb_image/**.h",
    "%{prj.name}/vendor/stb_image/**.cpp",
    "%{prj.name}/vendor/glm/glm/**.hpp",
    "%{prj.name}/vendor/glm/glm/**.inl",

}

includedirs
{
    "DeeDeeEngine/vendor/spdlog/include",
    "DeeDeeEngine/src",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.GLAD}",
    "%{IncludeDir.ImGui}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.stb_image}"
}

links
{
    "GLFW",
    "GLAD",
    "ImGui",
    "opengl32.lib"
}

filter "system:windows"
    staticruntime "On"
    systemversion "latest"

    defines {
    "DEE_PLATFORM_WINDOWS",
    "DEE_BUILD_DLL",
    "GLFW_INCLUDE_NONE"
}


filter "configurations:Debug"
defines "DEE_DEBUG"
runtime "Debug"
symbols "on"

filter "configurations:Release"
defines "DEE_RELEASE"
runtime "Release"
optimize "on"

filter "configurations:Dist"
defines "DEE_DIST"
runtime "Release"
optimize "on"

project "Sandbox"
location "Sandbox"
kind "ConsoleApp"
language "C++"
cppdialect "C++17"
staticruntime "on"

targetdir("bin/" .. outputdir .. "/%{prj.name}")
objdir("bin-int/" .. outputdir .. "/%{prj.name}")

files {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
}

includedirs
{
    "DeeDeeEngine/vendor/spdlog/include",
    "DeeDeeEngine/src",
    "DeeDeeEngine/vendor",
    "%{IncludeDir.glm}"
}
links{
    "DeeDeeEngine"
}

filter "system:windows"
    staticruntime "On"
    systemversion "latest"

    defines {
    "DEE_PLATFORM_WINDOWS",
}


filter "configurations:Debug"
defines "DEE_DEBUG"
symbols "on"

filter "configurations:Release"
defines "DEE_RELEASE"
optimize "on"

filter "configurations:Dist"
defines "DEE_DIST"
optimize "on"