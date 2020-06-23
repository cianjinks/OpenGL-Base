-- OpenGL-Sandbox
workspace "OpenGL-Sandbox"
	architecture "x64"
	startproject "OpenGL-Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to OpenGL-Base
IncludeDir = {}
IncludeDir["GLFW"] = "dependencies/GLFW/include"
IncludeDir["Glad"] = "dependencies/Glad/include"
IncludeDir["ImGui"] = "dependencies/imgui"
IncludeDir["glm"] = "dependencies/glm"
IncludeDir["stb_image"] = "dependencies/stb_image"

-- Projects
group "Dependencies"
	include "OpenGL-Base/dependencies/GLFW"
	include "OpenGL-Base/dependencies/Glad"
	include "OpenGL-Base/dependencies/imgui"
group ""

include "OpenGL-Base"
include "OpenGL-Sandbox"