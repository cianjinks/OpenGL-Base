#include "Shader.h"

namespace GLBase {

    Shader::Shader(const char* vertSrc, const char* fragSrc)
    {
        CreateProgram();
        AddShader(GL_VERTEX_SHADER, vertSrc);
        AddShader(GL_FRAGMENT_SHADER, fragSrc);
        Validate();
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_Program);
    }

    void Shader::Replace(const char* vertSrc, const char* fragSrc)
    {
        for (int i = 0; i < m_ShaderObjs.size(); i++)
        {
            glDetachShader(m_Program, m_ShaderObjs[i]);
        }
        AddShader(GL_VERTEX_SHADER, vertSrc);
        AddShader(GL_FRAGMENT_SHADER, fragSrc);
        Validate();
    }

    unsigned int Shader::GetProgram()
    {
        return m_Program;
    }

    void Shader::Bind()
    {
        glUseProgram(m_Program);
    }

    void Shader::UnBind()
    {
        glUseProgram(0);
    }

    void Shader::CreateProgram()
    {
        m_Program = glCreateProgram();
    }

    void Shader::AddShader(GLenum type, const char* path)
    {
        std::string source = ParseShaderFromFile(path);
        const char* rawsource = source.c_str();
        if (!source.empty())
        {
            unsigned int shaderObj = glCreateShader(type);
            glShaderSource(shaderObj, 1, &rawsource, nullptr);
            glCompileShader(shaderObj);
            ErrorHandleShader(shaderObj, m_Program);
            glAttachShader(m_Program, shaderObj);
            m_ShaderObjs.push_back(shaderObj);
            glDeleteShader(shaderObj);
        }
        else
        {
            std::cout << "[ERROR] Failed to open " << (type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader") << std::endl;
        }
    }

    void Shader::Validate()
    {
        glLinkProgram(m_Program);
        glValidateProgram(m_Program);
        glUseProgram(m_Program);
    }

    std::string Shader::ParseShaderFromFile(const char* filename)
    {
        std::ifstream in(filename, std::ios::in);
        if (in)
        {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return contents;
        }
        else
        {
            return "";
        }
    }

    void Shader::ErrorHandleShader(GLuint& shader, GLuint& program)
    {
        GLint result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            GLint length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            GLchar* log = (GLchar*)malloc(length);
            glGetShaderInfoLog(shader, length, &length, log);
            std::cout << "[ERROR] Failed to compile shader" << std::endl;
            std::cout << log << std::endl;
            free(log);
            glDeleteShader(shader);
            glDeleteProgram(program);
        }
    }

    void Shader::SetUniMat4f(const char* name, glm::mat4& matrix)
    {
        unsigned int location = glGetUniformLocation(m_Program, name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));

    }
}