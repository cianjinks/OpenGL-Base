#include "Sandbox.h"
#include <cstdlib>

Sandbox::Sandbox()
	: m_WindowWidth(1280.0f), m_WindowHeight(720.0f)
{
	m_Window = glfwCreateWindow((int)m_WindowWidth, (int)m_WindowHeight, "C++ OpenGL", NULL, NULL);
	glfwSetWindowPos(m_Window, 600, 200);
	//glfwSwapInterval(1); // Vsync
}

Sandbox::~Sandbox()
{
	delete m_Shader;
	delete m_IBO;
	delete m_VBO;
	delete m_LightSrcShader;
	delete m_LightSrcVBO;
	delete m_LightSrcIBO;
	delete vertices;
}

void Sandbox::Init()
{
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		GLBase::Application& application = *(GLBase::Application*)glfwGetWindowUserPointer(window);
		application.OnKeyPress(key, scancode, action, mods);
	});

	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
	{
		if (!m_Fullscreen)
		{
			glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, 1920, 1080, GLFW_DONT_CARE);
			m_Fullscreen = true;
		}
		else
		{
			glfwSetWindowMonitor(m_Window, NULL, 600, 200, (int)m_WindowWidth, (int)m_WindowHeight, GLFW_DONT_CARE);
			m_Fullscreen = false;
		}
	}
}

void Sandbox::OnMouseMove(float xpos, float ypos)
{
	if (glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
		if (m_InitialMouseMovement)
		{
			m_MouseX = xpos;
			m_MouseY = ypos;
			m_InitialMouseMovement = false;
		}

		float xoffset = xpos - m_MouseX;
		float yoffset = m_MouseY - ypos; // Y coordinate is top to bottom in glfw
		m_MouseX = xpos;
		m_MouseY = ypos;

		const float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_CameraYaw += xoffset;
		m_CameraPitch += yoffset;

		if (m_CameraPitch > 89.0f) {
			m_CameraPitch = 89.0f;
		}
		if (m_CameraPitch < -89.0f) {
			m_CameraPitch = -89.0f;
		}

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));
		direction.y = sin(glm::radians(m_CameraPitch));
		direction.z = sin(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));
		m_CameraFront = glm::normalize(direction);
	}
}

void Sandbox::OnMouseScroll(float xoffset, float yoffset)
{

}

void Sandbox::OnMouseClick(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		ImGuiIO& io = ImGui::GetIO();
		if (glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
			if (!io.WantCaptureMouse) {
				if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
					glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
			}
		}
	}
}

void Sandbox::ImGuiRender()
{
	ImGui::SetNextWindowSize(ImVec2(400, 385));
	ImGui::SetNextWindowPos(ImVec2(25, 25));
	ImGui::Begin("Camera Info: ");
	ImGui::Text(("X: " + std::to_string(m_CameraPos.x) + " Y: " + std::to_string(m_CameraPos.y) + " Z: " + std::to_string(m_CameraPos.z)).c_str());
	ImGui::Text(("Yaw: " + std::to_string(m_CameraYaw) + " Pitch: " + std::to_string(m_CameraPitch)).c_str());
	ImGui::SliderFloat("FOV", &m_CameraFOV, 30.0f, 120.0f);
	ImGui::Text(("Light | X: " + std::to_string(m_LightSrcPos.x) + " Y: " + std::to_string(m_LightSrcPos.y) + " Z: " + std::to_string(m_LightSrcPos.z)).c_str());
	ImGui::SliderFloat3("Ambience", &m_Ambient.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("Specular", &m_Specular.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("Diffuse", &m_Diffuse.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Shininess", &m_SpecularShininess, 1, 256);
	ImGui::SliderFloat3("Light Ambience", &m_LightAmbient.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("Light Specular", &m_LightSpecular.x, 0.0f, 1.0f);
	ImGui::SliderFloat3("Light Diffuse", &m_LightDiffuse.x, 0.0f, 1.0f);
	ImGui::ColorEdit3("Light Color", &m_LightSrcColor.x, 0);

	// TODO: Dosent fully work / Loses randomness when randomX hits 1
	if (ImGui::Button("Delete Random Cube", ImVec2(175, 20)))
	{
		if (m_TotalCubes > 0)
		{
			int randomX = (rand() % (m_xCubes - 1));
			int randomY = (rand() % (m_yCubes - 1));
			int cubeIndex = (20 * randomX) + randomY;
			int cubeElements = GLBase::Cube::VerticesPerCube * GLBase::Vertex::TotalElementCount;
			int firstElement = cubeElements * cubeIndex;
			vertices->erase(vertices->begin() + firstElement, vertices->begin() + firstElement + cubeElements);
			if (m_xCubes > 2)
			{
				m_xCubes--;
			}
			else if(m_yCubes > 2)
			{
				m_yCubes--;
			}
			m_TotalCubes--;
		}
	}

	ImGui::Text(("Frame Time: " + std::to_string(m_DeltaTime * 1000) + "ms | " + std::to_string((int)(1 / m_DeltaTime)) + "fps").c_str());
	ImGui::Text(("Vendor: " + std::string((char*)glGetString(GL_VENDOR))).c_str());
	ImGui::Text(("Renderer: " + std::string((char*)glGetString(GL_RENDERER))).c_str());
	ImGui::Text(("Version: " + std::string((char*)glGetString(GL_VERSION))).c_str());
	ImGui::End();
}

void Sandbox::Setup()
{
	/************************************CUBE TEST************************************************/
	vertices = new std::vector<float>();
	int xCubes = 10;
	int zCubes = 10;
	for (int x = -xCubes; x < xCubes; x++) {
		for (int z = -zCubes; z < zCubes; z++) {
			std::unique_ptr<GLBase::Cube> cube = std::make_unique<GLBase::Cube>(x, -2.0f, z, 1.0f);
			std::vector<GLBase::Quad*> quads = cube->GetQuads();

			for (GLBase::Quad* quad : quads) {
				std::vector<GLBase::Vertex*> quadVertices = quad->GetVertices();
				for (int i = 0; i < quadVertices.size(); i++)
				{
					std::vector<float> quadVertex = quadVertices.at(i)->GetVertex();
					for (int j = 0; j < GLBase::Vertex::TotalElementCount; j++)
					{
						vertices->emplace_back(quadVertex.at(j));
					}
				}
			}
		}
	}

	int totalCubes = 20 * 20;
	/***********************************************************************************************/

	float lightSrcVertices[] = {
		-0.25f + m_LightSrcPos.x, -0.25f + m_LightSrcPos.y, -0.25f + m_LightSrcPos.z,
		 0.25f + m_LightSrcPos.x, -0.25f + m_LightSrcPos.y, -0.25f + m_LightSrcPos.z,
		 0.25f + m_LightSrcPos.x,  0.25f + m_LightSrcPos.y, -0.25f + m_LightSrcPos.z,
		-0.25f + m_LightSrcPos.x,  0.25f + m_LightSrcPos.y, -0.25f + m_LightSrcPos.z,
		 0.25f + m_LightSrcPos.x, -0.25f + m_LightSrcPos.y,  0.25f + m_LightSrcPos.z,
		-0.25f + m_LightSrcPos.x, -0.25f + m_LightSrcPos.y,  0.25f + m_LightSrcPos.z,
		 0.25f + m_LightSrcPos.x,  0.25f + m_LightSrcPos.y,  0.25f + m_LightSrcPos.z,
		-0.25f + m_LightSrcPos.x,  0.25f + m_LightSrcPos.y,  0.25f + m_LightSrcPos.z,
		-0.25f + m_LightSrcPos.x,  0.25f + m_LightSrcPos.y,  0.25f + m_LightSrcPos.z,
		-0.25f + m_LightSrcPos.x,  0.25f + m_LightSrcPos.y, -0.25f + m_LightSrcPos.z,
		-0.25f + m_LightSrcPos.x, -0.25f + m_LightSrcPos.y, -0.25f + m_LightSrcPos.z,
		-0.25f + m_LightSrcPos.x, -0.25f + m_LightSrcPos.y,  0.25f + m_LightSrcPos.z,
		 0.25f + m_LightSrcPos.x,  0.25f + m_LightSrcPos.y,  0.25f + m_LightSrcPos.z,
		 0.25f + m_LightSrcPos.x,  0.25f + m_LightSrcPos.y, -0.25f + m_LightSrcPos.z,
		 0.25f + m_LightSrcPos.x, -0.25f + m_LightSrcPos.y, -0.25f + m_LightSrcPos.z,
		 0.25f + m_LightSrcPos.x, -0.25f + m_LightSrcPos.y,  0.25f + m_LightSrcPos.z,
		-0.25f + m_LightSrcPos.x, -0.25f + m_LightSrcPos.y, -0.25f + m_LightSrcPos.z,
		 0.25f + m_LightSrcPos.x, -0.25f + m_LightSrcPos.y, -0.25f + m_LightSrcPos.z,
		 0.25f + m_LightSrcPos.x, -0.25f + m_LightSrcPos.y,  0.25f + m_LightSrcPos.z,
		-0.25f + m_LightSrcPos.x, -0.25f + m_LightSrcPos.y,  0.25f + m_LightSrcPos.z,
		-0.25f + m_LightSrcPos.x,  0.25f + m_LightSrcPos.y, -0.25f + m_LightSrcPos.z,
		 0.25f + m_LightSrcPos.x,  0.25f + m_LightSrcPos.y, -0.25f + m_LightSrcPos.z,
		 0.25f + m_LightSrcPos.x,  0.25f + m_LightSrcPos.y,  0.25f + m_LightSrcPos.z,
		-0.25f + m_LightSrcPos.x,  0.25f + m_LightSrcPos.y,  0.25f + m_LightSrcPos.z
	};

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	m_VBO = new GLBase::VertexBuffer(*vertices);
	m_VBO->AddVertexAttribute(0, GLBase::Vertex::PositionElementCount, false, GLBase::Vertex::TotalElementCount, 0);
	m_VBO->AddVertexAttribute(1, GLBase::Vertex::TextureElementCount, false, GLBase::Vertex::TotalElementCount, GLBase::Vertex::PositionElementCount);
	m_VBO->AddVertexAttribute(2, GLBase::Vertex::NormalElementCount, false, GLBase::Vertex::TotalElementCount, GLBase::Vertex::PositionElementCount + GLBase::Vertex::TextureElementCount);
	m_VBO->ActivateVertexAttribute(0);
	m_VBO->ActivateVertexAttribute(1);
	m_VBO->ActivateVertexAttribute(2);

	int numFaces = 6 * totalCubes;
	int indicesCount = numFaces * 6; // 6 per face
	std::vector<unsigned int> indices(indicesCount);
	int j = 0;
	for (int i = 0; i < numFaces; i++)
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

	glGenVertexArrays(1, &lightSrcVaoID);
	glBindVertexArray(lightSrcVaoID);

	m_LightSrcVBO = new GLBase::VertexBuffer(lightSrcVertices, 24 * 6);
	m_LightSrcVBO->AddVertexAttribute(0, GLBase::Vertex::PositionElementCount, false, GLBase::Vertex::PositionElementCount, 0);
	m_LightSrcVBO->ActivateVertexAttribute(0);

	int lightSrcIndicesCount = 6 * 6; // 6 per face
	std::vector<unsigned int> lightSrcIndices(lightSrcIndicesCount);
	j = 0;
	for (int i = 0; i < 6; i++)
	{
		lightSrcIndices.emplace_back(j);
		lightSrcIndices.emplace_back(++j);
		lightSrcIndices.emplace_back(++j);
		lightSrcIndices.emplace_back(j);
		lightSrcIndices.emplace_back(++j);
		lightSrcIndices.emplace_back(j - 3);
		++j;
	}

	m_LightSrcIBO = new GLBase::IndexBuffer(lightSrcIndices);

	m_LightSrcShader = new GLBase::Shader("assets/shaders/lightvert.glsl", "assets/shaders/lightfrag.glsl");

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CW);
}

void Sandbox::Loop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	this->PollInput();

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO->GetBufferID());
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(float), vertices->data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(vaoID);
	m_Shader->Bind();

	// Model
	m_ModelMatrix = glm::mat4(1.0f);
	// View - Camera Calculations
	m_ViewMatrix = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
	// Projection
	m_ProjectionMatrix = glm::perspective(glm::radians(m_CameraFOV), m_WindowWidth / m_WindowHeight, 0.1f, 100.0f);
	// MVP
	glm::mat4 mvp = m_ProjectionMatrix * m_ViewMatrix * m_ModelMatrix;

	m_Shader->SetUniMat4f("u_TransformMatrix", mvp);
	m_Shader->SetUniMat4f("u_ModelMatrix", m_ModelMatrix);
	m_Shader->SetUniVec3f("u_ObjectColor", 1.0f, 0.5f, 0.31f);
	m_Shader->SetUniVec3f("u_LightColor", m_LightSrcColor.x, m_LightSrcColor.y, m_LightSrcColor.z);
	m_Shader->SetUniVec3f("u_Light.position", m_LightSrcPos.x, m_LightSrcPos.y, m_LightSrcPos.z);
	m_Shader->SetUniVec3f("u_CameraPos", m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
	m_Shader->SetUniVec3f("u_Material.ambient", m_Ambient.x, m_Ambient.y, m_Ambient.z);
	m_Shader->SetUniVec3f("u_Material.specular", m_Specular.x, m_Specular.y, m_Specular.z);
	m_Shader->SetUniVec3f("u_Material.diffuse", m_Diffuse.x, m_Diffuse.y, m_Diffuse.z);
	m_Shader->SetUniVec1f("u_Material.shininess", m_SpecularShininess);
	m_LightDiffuse = m_LightSrcColor * glm::vec3(0.5f);
	m_LightAmbient = m_LightDiffuse * glm::vec3(0.2f);
	m_Shader->SetUniVec3f("u_Light.ambient", m_LightAmbient.x, m_LightAmbient.y, m_LightAmbient.z);
	m_Shader->SetUniVec3f("u_Light.specular", m_LightSpecular.x, m_LightSpecular.y, m_LightSpecular.z);
	m_Shader->SetUniVec3f("u_Light.diffuse", m_LightDiffuse.x, m_LightDiffuse.y, m_LightDiffuse.z);

	glDrawElements(GL_TRIANGLES, m_IBO->GetNumIndices(), GL_UNSIGNED_INT, 0);

	m_Shader->UnBind();
	glBindVertexArray(0);

	glBindVertexArray(lightSrcVaoID);
	m_LightSrcShader->Bind();

	float lightSpeed = 2;
	m_LightSrcPos = glm::vec3(lightSpeed * sin(2 * glfwGetTime()), m_LightSrcPos.y, lightSpeed * cos(2 * glfwGetTime()));
	// Model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_LightSrcPos);
	// View
	glm::mat4 view = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
	// Projection
	glm::mat4 projection = glm::perspective(glm::radians(m_CameraFOV), m_WindowWidth / m_WindowHeight, 0.1f, 100.0f);
	// MVP
	mvp = projection * view * model;

	m_LightSrcShader->SetUniMat4f("u_TransformMatrix", mvp);
	m_LightSrcShader->SetUniVec3f("u_LightColor", m_LightSrcColor.x, m_LightSrcColor.y, m_LightSrcColor.z);
	glDrawElements(GL_TRIANGLES, m_LightSrcIBO->GetNumIndices(), GL_UNSIGNED_INT, 0);

	m_LightSrcShader->UnBind();
	glBindVertexArray(0);
}

void Sandbox::PollInput()
{
	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS) {
		m_CameraPos += m_CameraSpeed * m_CameraFront;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) {
		m_CameraPos -= m_CameraSpeed * m_CameraFront;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) {
		m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed;
	}
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) {
		m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed;
	}

	if (glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
	{
		m_CameraSpeed = 5.0f * m_DeltaTime;
	}
	else
	{
		m_CameraSpeed = 2.5f * m_DeltaTime;
	}
}

float Sandbox::GetWindowWidth()
{
	return m_WindowWidth;
}

float Sandbox::GetWindowHeight()
{
	return m_WindowHeight;
}