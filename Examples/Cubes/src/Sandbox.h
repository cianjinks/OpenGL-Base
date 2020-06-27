#pragma once
#include "GLBase.h"

class Sandbox : public GLBase::Application
{
private:
	float m_WindowWidth, m_WindowHeight;
	GLBase::Shader* m_Shader = nullptr;
	GLBase::Shader* m_SkyShader = nullptr;
	GLBase::IndexBuffer* m_IBO = nullptr;
	GLBase::VertexBuffer* m_VBO = nullptr;
	GLBase::Texture* m_Texture1 = nullptr;

	unsigned int vaoID = 0;

	// Camera
	glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float m_CameraYaw = -90.0f;
	float m_CameraPitch = 0.0f;
	float m_CameraFOV = 45.0f;
	float m_CameraSpeed = 2.5f * m_DeltaTime;

	float m_MouseX = m_WindowWidth / 2;
	float m_MouseY = m_WindowHeight / 2;

	bool m_InitialMouseMovement = true;
public:

	Sandbox();
	~Sandbox();
	void Init() override;
	void Loop() override;
	void Setup() override;
	void ImGuiRender() override;
	void OnKeyPress(int key, int scancode, int action, int mods) override;
	void OnMouseMove(float xpos, float ypos) override;
	void OnMouseScroll(float xoffset, float yoffset) override;
	void OnMouseClick(int button, int action, int mods) override;
	void PollInput();

	// Getters
	GLFWwindow* GetWindow() override { return m_Window; };
	float GetWindowWidth();
	float GetWindowHeight();
};

GLBase::Application* GLBase::CreateApplication()
{
	return new Sandbox();
}