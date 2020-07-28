#include "Sandbox.h"
#include <cstdlib>

Sandbox::Sandbox()
	: m_WindowWidth(1280.0f), m_WindowHeight(720.0f)
{
	m_Window = glfwCreateWindow((int)m_WindowWidth, (int)m_WindowHeight, "C++ OpenGL", NULL, NULL);
	glfwSetWindowPos(m_Window, 600, 200);
}

Sandbox::~Sandbox()
{
	delete m_Shader;
	delete m_IBO;
	delete m_VBO;
	delete m_Vertices;
	delete m_VoxelData;
}

void Sandbox::Init()
{
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		GLBase::Application& application = *(GLBase::Application*)glfwGetWindowUserPointer(window);
		application.OnKeyPress(key, scancode, action, mods);
	});

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
		GLBase::Application& application = *(GLBase::Application*)glfwGetWindowUserPointer(window);
		application.OnMouseMove((float)xpos, (float)ypos);
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
		GLBase::Application& application = *(GLBase::Application*)glfwGetWindowUserPointer(window);
		application.OnMouseScroll((float)xoffset, (float)yoffset);
	});
	
	glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
		GLBase::Application& application = *(GLBase::Application*)glfwGetWindowUserPointer(window);
		application.OnMouseClick(button, action, mods);
	});

	glViewport(0, 0, (GLsizei)m_WindowWidth, (GLsizei)m_WindowHeight);
	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	});
}

void Sandbox::OnKeyPress(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		uint32_t array[] = { 70142, 78334, 94718, 127486, 74238, 82430, 98814, 66302, 66814, 67838 };
		if (t < 9) {
			m_VoxelData[0] = array[t];
			t++;
		}
	}
}

void Sandbox::OnMouseMove(float xpos, float ypos)
{
}

void Sandbox::OnMouseScroll(float xoffset, float yoffset)
{

}

void Sandbox::OnMouseClick(int button, int action, int mods)
{
}

void Sandbox::ImGuiRender()
{
	ImGui::SetNextWindowPos(ImVec2(25, 25));
	ImGui::Begin("Info: ");
	ImGui::Text(("Frame Time: " + std::to_string(m_DeltaTime * 1000) + "ms | " + std::to_string((int)(1 / m_DeltaTime)) + "fps").c_str());
	ImGui::Text(("Vendor: " + std::string((char*)glGetString(GL_VENDOR))).c_str());
	ImGui::Text(("Renderer: " + std::string((char*)glGetString(GL_RENDERER))).c_str());
	ImGui::Text(("Version: " + std::string((char*)glGetString(GL_VERSION))).c_str());
	ImGui::End();
}

void Sandbox::Setup()
{
	glfwSwapInterval(0); // Vsync
	m_Vertices = new float[8]
	{
		0.0f, 0.0f,
		m_WindowWidth, 0.0f,
		m_WindowWidth, m_WindowHeight,
		0.0f, m_WindowHeight
	};

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	m_VBO = new GLBase::VertexBuffer(m_Vertices, 8);
	m_VBO->AddVertexAttribute(0, 2, false, 2, 0);
	m_VBO->ActivateVertexAttribute(0);

	std::vector<unsigned int> indices(6);
	int j = 0;
	for (int i = 0; i < 6; i++)
	{
		indices.emplace_back(j);
		indices.emplace_back(++j);
		indices.emplace_back(++j);
		indices.emplace_back(j);
		indices.emplace_back(++j);
		indices.emplace_back(j - 3);
		++j;
	}

	m_IBO = new GLBase::IndexBuffer(indices);

	m_Shader = new GLBase::Shader("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");

	glEnable(GL_DEPTH_TEST);

	// BUFFER TEXTURE TEST
	//uint32_t array[] = { 129526, 197373, 312318, 1791, 329726, 1023 };
	m_VoxelData = new uint32_t[230]
	{ 66046, 196352, 719365, 1111306, 1504267, 1798287, 2010960, 2383776, 2760672, 3081212, 3244159, 3317855, 3448927, 3604224, 4127237, 4521728, 4997311, 5075087, 5308160, 5821483, 6094592, 6583690, 6924885, 7205389, 7533324, 7879365, 8190469, 8581900, 8917486, 9045243, 9153109, 9436928, 9961216, 10485504, 10945277, 11012853, 11206400, 11685298, 11992832, 12474280, 12782835, 12914672, 13238016, 13729664, 14169036, 14418430, 14484221, 14565312, 14946798, 33023, 57599, 12543, 52479, 41727, 51455, 52479, 8447, 65535, 52479, 52479, 65535, 65535, 57599, 61695, 35071, 65535, 43775, 65535, 61695, 65535, 62207, 65535, 65535, 65535, 65535, 65535, 49407, 61695, 20735, 12543, 52479, 50431, 65535, 65535, 52479, 52479, 65535, 65535, 20735, 50431, 65535, 54783, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 62975, 28927, 65535, 12543, 65535, 33023, 61695, 56831, 14335, 12543, 60159, 65535, 47871, 65535, 61695, 61695, 65535, 65535, 29695, 12543, 65535, 511, 8447, 47103, 49407, 63743, 65535, 19711, 8959, 1535, 61695, 61695, 65535, 65535, 13311, 12799, 4351, 30719, 5631, 35071, 35071, 35071, 767, 52479, 52479, 65535, 65535, 52479, 52479, 8191, 49151, 35583, 65535, 44031, 65535, 2303, 36863, 2815, 45055, 65535, 65535, 65535, 65535, 53247, 61439, 65535, 65535, 3327, 2815, 767, 52479, 52479, 65535, 65535, 52479, 52479, 65535, 65535, 52479, 65535, 56831, 19967, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 13311, 65535, 24575, 24575, 3327, 1279, 4095, 3583, 4095, 511, 61183, 65535, 65535, 65535, 61183, 57343, 49151, 32767, 65535, 65535, 22527, 1023, 4095, 4095, 511, 13311, 13311, 767, 1023, 4607, 1279, 65535, 65535, 13311, 13311, 16383, 8191, 30719, 511 };
	unsigned int bufferData;
	glGenBuffers(1, &bufferData);
	glBindBuffer(GL_TEXTURE_BUFFER, bufferData);
	glBufferData(GL_TEXTURE_BUFFER, 230 * sizeof(uint32_t), m_VoxelData, GL_DYNAMIC_DRAW);

	unsigned int textureBuffer;
	glGenTextures(1, &textureBuffer);
	glBindTexture(GL_TEXTURE_BUFFER, textureBuffer);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32UI, bufferData);
}

void Sandbox::Loop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	this->PollInput();

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO->GetBufferID());
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), m_Vertices, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_TEXTURE_BUFFER, 0, 230 * sizeof(uint32_t), m_VoxelData);

	glBindVertexArray(vaoID);
	m_Shader->Bind();

	// Model
	m_ModelMatrix = glm::mat4(1.0f);
	// View - Camera Calculations
	m_ViewMatrix = glm::mat4(1.0f);
	// Projection
	m_ProjectionMatrix = glm::ortho(0.0f, m_WindowWidth, 0.0f, m_WindowHeight);
	// MVP
	glm::mat4 mvp = m_ProjectionMatrix * m_ViewMatrix * m_ModelMatrix;

	m_Shader->SetUniMat4f("u_MVP", mvp);
	m_Shader->SetUniVec1f("u_Time", (float)glfwGetTime());

	glDrawElements(GL_TRIANGLES, m_IBO->GetNumIndices(), GL_UNSIGNED_INT, 0);

	m_Shader->UnBind();
	glBindVertexArray(0);
}

void Sandbox::PollInput()
{
}

float Sandbox::GetWindowWidth()
{
	return m_WindowWidth;
}

float Sandbox::GetWindowHeight()
{
	return m_WindowHeight;
}