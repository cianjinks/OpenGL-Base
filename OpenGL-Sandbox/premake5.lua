project "OpenGL-Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"../OpenGL-Base/src",
		"../OpenGL-Base/dependencies",
		"../OpenGL-Base/%{IncludeDir.glm}",
		"../OpenGL-Base/%{IncludeDir.Glad}",
		"../OpenGL-Base/%{IncludeDir.ImGui}",
		"../OpenGL-Base/%{IncludeDir.GLFW}"
	}

	links
	{
		"OpenGL-Base"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLCORE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "GLCORE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GLCORE_RELEASE"
		runtime "Release"
        optimize "on"