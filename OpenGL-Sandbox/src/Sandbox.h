#pragma once
#include "GLBase.h"

class Sandbox : public GLBase::Application
{
private:
	GLFWwindow* m_Window;
	int m_WindowWidth, m_WindowHeight;
	GLBase::Shader* m_Shader;
	GLBase::IndexBuffer* m_IBO;
	unsigned int m_VBO;
public:

	Sandbox();
	~Sandbox();
	void Init() override;
	void Loop() override;
	void Setup() override;
	void OnKeyPress(int key, int scancode, int action, int mods) override;
	GLFWwindow* getWindow() override { return m_Window; };
};

GLBase::Application* GLBase::CreateApplication()
{
	return new Sandbox();
}