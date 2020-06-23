#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

namespace GLBase {
	class Shader
	{
	private:
		unsigned int m_Program;
	public:
		Shader(const char* vertSrc, const char* fragSrc);
		~Shader();
		unsigned int GetProgram();
		void Bind();
		void UnBind();
	private:
		void CreateProgram();
		void AddShader(GLenum type, const char* path);
		void Validate();
        static std::string ParseShaderFromFile(const char* filename);
		static void ErrorHandleShader(GLuint& shader, GLuint& program);
	};

}