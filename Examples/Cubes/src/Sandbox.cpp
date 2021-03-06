#include "Sandbox.h"

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
	delete m_Texture1;
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
		glfwSetWindowShouldClose(m_Window, true);
	}
	if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
	ImGuiIO& io = ImGui::GetIO();
	if (glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
		if (!io.WantCaptureMouse) {
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	}
}

void Sandbox::ImGuiRender()
{
	ImGui::SetNextWindowSize(ImVec2(275, 160));
	ImGui::SetNextWindowPos(ImVec2(25, 25));
	ImGui::Begin("Camera Info: ");
	ImGui::Text(("X: " + std::to_string(m_CameraPos.x) + " Y: " + std::to_string(m_CameraPos.y) + " Z: " + std::to_string(m_CameraPos.z)).c_str());
	ImGui::Text(("Yaw: " + std::to_string(m_CameraYaw) + " Pitch: " + std::to_string(m_CameraPitch)).c_str());
	ImGui::SliderFloat("FOV", &m_CameraFOV, 30.0f, 120.0f);
	ImGui::Text(("Frame Time: " + std::to_string(m_DeltaTime * 1000) + "ms | " + std::to_string((int)(1 / m_DeltaTime)) + "fps").c_str());
	ImGui::Text(("Vendor: " + std::string((char*)glGetString(GL_VENDOR))).c_str());
	ImGui::Text(("Renderer: " + std::string((char*)glGetString(GL_RENDERER))).c_str());
	ImGui::Text(("Version: " + std::string((char*)glGetString(GL_VERSION))).c_str());
	ImGui::End();
}

void Sandbox::Setup()
{
	int numVertices = 5 * 24; 

	std::vector<float> vertices = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f
	};

	/**CUBE TEST**/
	int xCubes = 50;
	int yCubes = 50;
	int zCubes = 50;
	int calculateVertices = xCubes * yCubes * zCubes * 6 * 4 * 5;
	int processedVertices = 0;
	int calculateCubes = xCubes * yCubes * zCubes;
	int processedCubes = 0;
	for (int x = -xCubes; x < xCubes; x++) {
		for (int z = -zCubes; z < zCubes; z++) {
			std::unique_ptr<GLBase::Cube> cube = std::make_unique<GLBase::Cube>(x, 1.0f, z, 1.0f);
			std::vector<GLBase::Quad*> quads = cube->GetQuads();

			for (GLBase::Quad* quad : quads) {
				std::vector<GLBase::Vertex*> quadVertices = quad->GetVertices();
				for (int i = 0; i < quadVertices.size(); i++)
				{
					std::vector<float> quadVertex = quadVertices.at(i)->GetVertex();
					for (int j = 0; j < GLBase::Vertex::TotalElementCount; j++)
					{
						vertices.emplace_back(quadVertex.at(j));
						processedVertices++;
						//std::cout << "[LOADING] Processed Vertices: (" << processedVertices << "/" << calculateVertices << ") | ";
						//std::cout << "Processed Cubes: (" << processedCubes << "/" << calculateCubes << ")";
						//std::cout << "\r\r";
					}
				}
			}
			processedCubes++;
		}
	}
	std::cout << std::endl;
	/*************/

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	m_VBO = new GLBase::VertexBuffer(vertices);
	m_VBO->AddVertexAttribute(0, GLBase::Vertex::PositionElementCount, false, GLBase::Vertex::TotalElementCount, 0);
	m_VBO->AddVertexAttribute(1, GLBase::Vertex::TextureElementCount, false, GLBase::Vertex::TotalElementCount, GLBase::Vertex::PositionElementCount);
	m_VBO->ActivateVertexAttribute(0);
	m_VBO->ActivateVertexAttribute(1);

	int numFaces = 6 * 2 * xCubes * 2 * zCubes + 6;
	int processedIndices = 0;
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
		processedIndices += 6;
		//std::cout << "[LOADING] Processed Indices: (" << processedIndices << "/" << indicesCount << ")";
		//std::cout << "\r";
	}

	m_IBO = new GLBase::IndexBuffer(indices);

	m_Texture1 = new GLBase::Texture("assets/textures/white-tile.jpg", true);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_Shader = new GLBase::Shader("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CW);
}

void Sandbox::Loop()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.76f, 1.0f, 1.0f);
	this->PollInput();

	glBindVertexArray(vaoID);
	m_Texture1->Bind();
	m_Shader->Bind();

	// CUBE 1

	// Model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	// View - Camera Calculations
	glm::mat4 view = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
	// Projection
	glm::mat4 projection = glm::perspective(glm::radians(m_CameraFOV), m_WindowWidth / m_WindowHeight, 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(0.0f, 16.0f, 0.0f, 9.0f, 0.1f, 100.0f);
	// MVP
	glm::mat4 mvp = projection * view * model;

	m_Shader->SetUniMat4f("u_TransformMatrix", mvp);
	glDrawElements(GL_TRIANGLES, m_IBO->GetNumIndices(), GL_UNSIGNED_INT, 0);
#if 0	
	// CUBE 2
	
	// Model
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.5f, 0.0f));
	//model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	// MVP
	mvp = projection * view * model;

	m_Shader->SetUniMat4f("u_TransformMatrix", mvp);
	glDrawElements(GL_TRIANGLES, m_IBO->GetNumIndices(), GL_UNSIGNED_INT, 0);

	// CUBE 3
	// Model
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.0f, 0.5f, 0.0f));
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f) * 6, glm::vec3(0.5f, 1.0f, 0.0f));
	// MVP
	mvp = projection * view * model;

	m_Shader->SetUniMat4f("u_TransformMatrix", mvp);
	glDrawElements(GL_TRIANGLES, m_IBO->GetNumIndices(), GL_UNSIGNED_INT, 0);
#endif
	m_Shader->UnBind();
	m_Texture1->UnBind();
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