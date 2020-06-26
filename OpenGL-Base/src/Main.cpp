#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include <iostream>
#include "Application.h"

#define PRINT(x) std::cout << x << std::endl;

extern GLBase::Application* GLBase::CreateApplication();

int main()
{
	if (!glfwInit())
	{
		PRINT("[ERROR] Failed to initialise GLFW");
	}
	else
	{
		PRINT("[SUCCESS] Initialised GLFW")
	}

	GLBase::Application* app = GLBase::CreateApplication();
	GLFWwindow* window = app->GetWindow();

	if (!window)
	{
		glfwTerminate();
		PRINT("[ERROR] Failed to create a window");
	}
	else
	{
		PRINT("[SUCCESS] Created window")
	}

	glfwMakeContextCurrent(window);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!status)
	{
		PRINT("[Error] Failed to initialise Glad");
	}
	else
	{
		PRINT("[SUCCESS] Initialised GLAD");
	}

	app->Init();
	app->Setup();

	/****** IMGUI ******/
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
	/*******************/

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = (float)glfwGetTime();
		app->m_DeltaTime = currentFrame - app->m_LastFrame;
		app->m_LastFrame = currentFrame;
		app->Loop();

		/****** IMGUI ******/
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		app->ImGuiRender();
		io.DisplaySize = ImVec2(app->GetWindowWidth() , app->GetWindowHeight());
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		/*******************/
		
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	delete app;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}