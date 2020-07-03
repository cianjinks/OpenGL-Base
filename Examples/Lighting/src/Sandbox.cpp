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
	delete m_SkyVBO;
	delete m_SkyIBO;
	delete m_LightSrcShader;
	delete m_LightSrcVBO;
	delete m_LightSrcIBO;
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

	std::vector<float> vertices;
	/**CUBE TEST**/
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
						vertices.emplace_back(quadVertex.at(j));
					}
				}
			}
		}
	}

	int totalCubes = 20 * 20;

	/*************/

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	m_VBO = new GLBase::VertexBuffer(vertices);
	m_VBO->AddVertexAttribute(0, GLBase::Vertex::PositionElementCount, false, GLBase::Vertex::TotalElementCount, 0);
	m_VBO->AddVertexAttribute(1, GLBase::Vertex::TextureElementCount, false, GLBase::Vertex::TotalElementCount, GLBase::Vertex::PositionElementCount);
	m_VBO->AddVertexAttribute(2, GLBase::Vertex::NormalElementCount, false, GLBase::Vertex::TotalElementCount, GLBase::Vertex::PositionElementCount + GLBase::Vertex::TextureElementCount);	
	m_VBO->ActivateVertexAttribute(0);
	m_VBO->ActivateVertexAttribute(1);
	m_VBO->ActivateVertexAttribute(2);

	int numFaces = 6 * totalCubes;
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

	m_Texture1 = new GLBase::Texture("assets/textures/brick.jpg", true);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_Shader = new GLBase::Shader("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");

	float skyboxVertices[] = {
		// positions          
		-1.0f, -1.0f, -1.0f,  
		 1.0f, -1.0f, -1.0f,  
		 1.0f,  1.0f, -1.0f,  
		-1.0f,  1.0f, -1.0f,  
		-1.0f, -1.0f,  1.0f,  
		 1.0f, -1.0f,  1.0f,  
		 1.0f,  1.0f,  1.0f,  
		-1.0f,  1.0f,  1.0f,  
		-1.0f,  1.0f,  1.0f,  
		-1.0f,  1.0f, -1.0f,  
		-1.0f, -1.0f, -1.0f,  
		-1.0f, -1.0f,  1.0f,  
		 1.0f,  1.0f,  1.0f,  
		 1.0f,  1.0f, -1.0f,  
		 1.0f, -1.0f, -1.0f,  
		 1.0f, -1.0f,  1.0f,  
		-1.0f, -1.0f, -1.0f,  
		 1.0f, -1.0f, -1.0f,  
		 1.0f, -1.0f,  1.0f,  
		-1.0f, -1.0f,  1.0f,  
		-1.0f,  1.0f, -1.0f,  
		 1.0f,  1.0f, -1.0f,  
		 1.0f,  1.0f,  1.0f,  
		-1.0f,  1.0f,  1.0f
	};

	glGenVertexArrays(1, &skyVaoID);
	glBindVertexArray(skyVaoID);

	m_SkyVBO = new GLBase::VertexBuffer(skyboxVertices, 6 * 12);
	m_SkyVBO->AddVertexAttribute(0, GLBase::Vertex::PositionElementCount, false, GLBase::Vertex::PositionElementCount, 0);
	m_SkyVBO->ActivateVertexAttribute(0);

	int skyIndicesCount = 6 * 6; // 6 per face
	std::vector<unsigned int> skyIndices(skyIndicesCount);
	j = 0;
	for (int i = 0; i < 6; i++)
	{
		skyIndices.emplace_back(j);
		skyIndices.emplace_back(++j);
		skyIndices.emplace_back(++j);
		skyIndices.emplace_back(j);
		skyIndices.emplace_back(++j);
		skyIndices.emplace_back(j - 3);
		++j;
	}

	m_SkyIBO = new GLBase::IndexBuffer(skyIndices);

	std::vector<std::string> faces = {
		"assets/textures/sky/right.jpg",
		"assets/textures/sky/left.jpg",
		"assets/textures/sky/top.jpg",
		"assets/textures/sky/bottom.jpg",
		"assets/textures/sky/front.jpg",
		"assets/textures/sky/back.jpg"
	};

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "[ERROR] Failed to load cubemap texture: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	m_SkyShader = new GLBase::Shader("assets/shaders/skyvert.glsl", "assets/shaders/skyfrag.glsl");

	std::vector<float> lightSourceVertices;
	std::unique_ptr<GLBase::Cube> lightSource = std::make_unique<GLBase::Cube>(m_LightSrcPos.x, m_LightSrcPos.y, m_LightSrcPos.z, 0.25f);
	std::vector<GLBase::Quad*> quads = lightSource->GetQuads();

	for (GLBase::Quad* quad : quads) {
		std::vector<GLBase::Vertex*> quadVertices = quad->GetVertices();
		for (int i = 0; i < quadVertices.size(); i++)
		{
			std::vector<float> quadVertex = quadVertices.at(i)->GetVertex();
			for (int j = 0; j < GLBase::Vertex::TotalElementCount; j++)
			{
				lightSourceVertices.emplace_back(quadVertex.at(j));
			}
		}
	}

	glGenVertexArrays(1, &lightSrcVaoID);
	glBindVertexArray(lightSrcVaoID);

	m_LightSrcVBO = new GLBase::VertexBuffer(lightSourceVertices);
	m_LightSrcVBO->AddVertexAttribute(0, GLBase::Vertex::PositionElementCount, false, GLBase::Vertex::TotalElementCount, 0);
	m_LightSrcVBO->AddVertexAttribute(1, GLBase::Vertex::TextureElementCount, false, GLBase::Vertex::TotalElementCount, GLBase::Vertex::PositionElementCount);
	m_LightSrcVBO->AddVertexAttribute(2, GLBase::Vertex::NormalElementCount, false, GLBase::Vertex::TotalElementCount, GLBase::Vertex::PositionElementCount + GLBase::Vertex::TextureElementCount);
	m_LightSrcVBO->ActivateVertexAttribute(0);
	m_LightSrcVBO->ActivateVertexAttribute(1);
	m_LightSrcVBO->ActivateVertexAttribute(2);

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
	glClearColor(0.0f, 0.76f, 1.0f, 1.0f);
	this->PollInput();

	glDepthMask(GL_FALSE);
	glBindVertexArray(skyVaoID);
	m_SkyShader->Bind();

	glm::mat4 skyModel = glm::mat4(1.0f);
	glm::mat4 skyView = glm::mat4(glm::mat3(glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp)));
	glm::mat4 skyProjection = glm::perspective(glm::radians(m_CameraFOV), m_WindowWidth / m_WindowHeight, 0.1f, 100.0f);
	// MVP
	glm::mat4 skyMvp = skyProjection * skyView * skyModel;

	m_SkyShader->SetUniMat4f("u_TransformMatrix", skyMvp);
	glDrawElements(GL_TRIANGLES, m_IBO->GetNumIndices(), GL_UNSIGNED_INT, 0);
	glDepthMask(GL_TRUE);

	m_SkyShader->UnBind();
	glBindVertexArray(0);

	glBindVertexArray(vaoID);
	m_Texture1->Bind();
	m_Shader->Bind();

	// CUBE 1

	// Model
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
	//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	// View - Camera Calculations
	glm::mat4 view = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
	// Projection
	glm::mat4 projection = glm::perspective(glm::radians(m_CameraFOV), m_WindowWidth / m_WindowHeight, 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(0.0f, 16.0f, 0.0f, 9.0f, 0.1f, 100.0f);
	// MVP
	glm::mat4 mvp = projection * view * model;

	m_Shader->SetUniMat4f("u_TransformMatrix", mvp);
	m_Shader->SetUniMat4f("u_ModelMatrix", model);
	m_Shader->SetUniVec3f("u_ObjectColor", 1.0f, 0.5f, 0.31f);
	m_Shader->SetUniVec3f("u_LightColor", 1.0f, 1.0f, 1.0f);
	m_Shader->SetUniVec3f("u_LightPos", m_LightSrcPos.x, m_LightSrcPos.y, m_LightSrcPos.z);
	m_Shader->SetUniVec3f("u_CameraPos", m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
	glDrawElements(GL_TRIANGLES, m_IBO->GetNumIndices(), GL_UNSIGNED_INT, 0);

	m_Shader->UnBind();
	m_Texture1->UnBind();
	glBindVertexArray(0);

	glBindVertexArray(lightSrcVaoID);
	m_LightSrcShader->Bind();

	// Model
	glm::mat4 lightSrcModel = glm::mat4(1.0f);
	// View - Camera Calculations
	glm::mat4 lightSrcView = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
	// Projection
	glm::mat4 lightSrcProjection = glm::perspective(glm::radians(m_CameraFOV), m_WindowWidth / m_WindowHeight, 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(0.0f, 16.0f, 0.0f, 9.0f, 0.1f, 100.0f);
	// MVP
	glm::mat4 lightSrcMvp = lightSrcProjection * lightSrcView * lightSrcModel;

	m_LightSrcShader->SetUniMat4f("u_TransformMatrix", lightSrcMvp);
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