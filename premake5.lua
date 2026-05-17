workspace "XTEngine2d"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
include "XTEngine2d/vendor/glfw"
group ""

---------------------------------------------------------
-- XTEngine2d
---------------------------------------------------------

project "XTEngine2d"
    location "XTEngine2d"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"

    staticruntime "Off"
    systemversion "latest"

    buildoptions { "/utf-8" }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",

        "%{prj.name}/vendor/glad/src/glad.c",

        "%{prj.name}/vendor/imgui/*.cpp",
        "%{prj.name}/vendor/imgui/backends/imgui_impl_glfw.cpp",
        "%{prj.name}/vendor/imgui/backends/imgui_impl_opengl3.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",

        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/glm",

        "%{prj.name}/vendor/glfw/include",
        "%{prj.name}/vendor/glad/include",

        "%{prj.name}/vendor/json",
        "%{prj.name}/vendor/portable-file-dialogs",

        "%{prj.name}/vendor/imgui",
        "%{prj.name}/vendor/imgui/backends"
    }

    defines
    {
        "XT_PLATFORM_WINDOWS"
    }

    links
    {
        "GLFW",
        "opengl32"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "XT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "XT_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "XT_DIST"
        optimize "On"

    filter {}

---------------------------------------------------------
-- Sandbox
---------------------------------------------------------

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"

    language "C++"
    cppdialect "C++20"

    staticruntime "Off"
    systemversion "latest"

    buildoptions { "/utf-8" }

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "XTEngine2d/src",

        "XTEngine2d/vendor/spdlog/include",
        "XTEngine2d/vendor/glm",

        "XTEngine2d/vendor/glfw/include",
        "XTEngine2d/vendor/glad/include",

        "XTEngine2d/vendor/json",
        "XTEngine2d/vendor/imgui",
        "XTEngine2d/vendor/imgui/backends",
        "XTEngine2d/vendor/portable-file-dialogs"
    }

    links
    {
        "XTEngine2d"
    }

    defines
    {
        "XT_PLATFORM_WINDOWS"
    }

    filter "configurations:Debug"
        defines "XT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "XT_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "XT_DIST"
        optimize "On"

    filter {}