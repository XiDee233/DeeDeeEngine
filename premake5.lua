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
IncludeDir["GLAD"] = "DeeDeeEngine/vendor/GLAD/include"
IncludeDir["ImGui"] = "DeeDeeEngine/vendor/imgui"
IncludeDir["glm"] = "DeeDeeEngine/vendor/glm"


include "DeeDeeEngine/vendor/GLFW"
include "DeeDeeEngine/vendor/GLAD"
include "DeeDeeEngine/vendor/ImGui"

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
    "%{IncludeDir.glm}"


}

links
{
    "GLFW",
    "GLAD",
    "ImGui",
    "opengl32.lib"
}

filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    defines {
    "DEE_PLATFORM_WINDOWS",
    "DEE_BUILD_DLL",
    "GLFW_INCLUDE_NONE"
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
    "DeeDeeEngine/src",
    "DeeDeeEngine/vendor",
    "%{IncludeDir.glm}"
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