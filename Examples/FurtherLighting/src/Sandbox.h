#pragma once
#include "GLBase.h"

class Sandbox : public GLBase::Application
{
private:
	float m_WindowWidth, m_WindowHeight;
	bool m_Fullscreen = false;

	// Cubes
	std::vector<float>* vertices;
	int m_xCubes = 20;
	int m_yCubes = 20;
	int m_TotalCubes = 20 * 20;

	GLBase::Shader* m_Shader = nullptr;
	GLBase::Shader* m_LightSrcShader = nullptr;
	GLBase::IndexBuffer* m_IBO = nullptr;
	GLBase::IndexBuffer* m_LightSrcIBO = nullptr;
	GLBase::VertexBuffer* m_VBO = nullptr;
	GLBase::VertexBuffer* m_LightSrcVBO = nullptr;

	unsigned int vaoID = 0;
	unsigned int lightSrcVaoID = 0;

	// MVP
	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
	glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
	glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);

	// Camera
	glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float m_CameraYaw = -90.0f;
	float m_CameraPitch = 0.0f;
	float m_CameraFOV = 60.0f;
	float m_CameraSpeed = 2.5f * m_DeltaTime;

	float m_MouseX = m_WindowWidth / 2;
	float m_MouseY = m_WindowHeight / 2;

	bool m_InitialMouseMovement = true;

	// Lighting
	glm::vec3 m_LightSrcPos = glm::vec3(0.0f, 2.0f, 2.0f);
	glm::vec3 m_LightSrcColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 m_Ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 m_Diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 m_Specular = glm::vec3(0.5f);
	glm::vec3 m_LightAmbient = glm::vec3(0.2f);
	glm::vec3 m_LightDiffuse = glm::vec3(0.5f);
	glm::vec3 m_LightSpecular = glm::vec3(1.0f);
	float m_SpecularShininess = 32.0f;

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