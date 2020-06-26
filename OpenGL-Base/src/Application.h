#pragma once
#include "IndexBuffer.h"
#include "Shader.h"

namespace GLBase {
	class Application
	{

	public:
		GLFWwindow* m_Window = nullptr;
		float m_DeltaTime = 0.0f;
		float m_LastFrame = 0.0f;

		virtual void Init() = 0;
		virtual void Loop() = 0;
		virtual void Setup() = 0;
		virtual void ImGuiRender() = 0;
		virtual void OnKeyPress(int key, int scancode, int action, int mods) = 0;
		virtual void OnMouseMove(float xpos, float ypos) = 0;
		virtual void OnMouseScroll(float xoffset, float yoffset) = 0;
		virtual void OnMouseClick(int button, int action, int mods) = 0;

		// Getters
		virtual GLFWwindow* GetWindow() = 0;
		virtual float GetWindowWidth() = 0;
		virtual float GetWindowHeight() = 0;
	};

	Application* CreateApplication();
}
