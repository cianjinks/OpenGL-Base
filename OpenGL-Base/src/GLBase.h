#pragma once
#define PRINT(x) std::cout << x << std::endl;
// Dependencies
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"
#include "imgui.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Custom Classes
#include "IndexBuffer.h"
#include "Shader.h"
#include "Application.h"