#include "glad/glad.h"
#include "GLFW/glfw3.h"

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
	GLFWwindow* window = app->getWindow();

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

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		app->Loop();
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	delete app;
	return 0;
}