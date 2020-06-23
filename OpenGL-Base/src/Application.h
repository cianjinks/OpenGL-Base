#pragma once
#include "IndexBuffer.h"
#include "Shader.h"

namespace GLBase {
	class Application
	{

	public:
		virtual void Init() = 0;
		virtual void Loop() = 0;
		virtual void Setup() = 0;
		virtual void OnKeyPress(int key, int scancode, int action, int mods) = 0;
		virtual GLFWwindow* getWindow() = 0;
	};

	Application* CreateApplication();
}
