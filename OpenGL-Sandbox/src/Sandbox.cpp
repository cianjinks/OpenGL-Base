#include "Sandbox.h"

Sandbox::Sandbox()
	: m_WindowWidth(960), m_WindowHeight(960), m_Shader(nullptr), m_IBO(nullptr), m_VBO(0)
{
	m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "C++ OpenGL", NULL, NULL);
}

Sandbox::~Sandbox()
{
	glfwTerminate();
	delete m_Shader;
	delete m_IBO;
}

void Sandbox::Init()
{
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		GLBase::Application& application = *(GLBase::Application*)glfwGetWindowUserPointer(window);
		application.OnKeyPress(key, scancode, action, mods);
		});

	glViewport(0, 0, m_WindowWidth, m_WindowHeight);
	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		});
}

void Sandbox::OnKeyPress(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
	{
		glfwSetWindowShouldClose(m_Window, true);
	}
}

void Sandbox::Setup()
{
	float vertices[8] = {
		-1.0f,  1.0f,
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f
	};

	unsigned int vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	unsigned int indices[6] = {
		0, 1, 2,
		2, 3, 0
	};

	m_IBO = new GLBase::IndexBuffer(indices, 6);
	m_Shader = new GLBase::Shader("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");
}

void Sandbox::Loop()
{
	m_IBO->Bind();
	m_Shader->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	m_Shader->UnBind();
	m_IBO->UnBind();
}
