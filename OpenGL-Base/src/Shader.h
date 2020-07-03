#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace GLBase {
	class Shader
	{
	private:
		unsigned int m_Program;
		std::vector<unsigned int> m_ShaderObjs;
	public:
		Shader(const char* vertSrc, const char* fragSrc);
		~Shader();
		unsigned int GetProgram();
		void Bind();
		void UnBind();
		void Replace(const char* vertSrc, const char* fragSrc);

		// Uniforms
		void SetUniMat4f(const char* name, glm::mat4& matrix);
		void SetUniVec3f(const char* name, float x, float y, float z);
		void SetUniVec1f(const char* name, float f);
		void SetUniVec1i(const char* name, int i);
	private:
		void CreateProgram();
		void Validate();
		void AddShader(GLenum type, const char* path);
        static std::string ParseShaderFromFile(const char* filename);
		static void ErrorHandleShader(GLuint& shader, GLuint& program);
	};
}