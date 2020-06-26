#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Texture.h"
#include "stb_image.h"

#include <iostream>

namespace GLBase {

	int Texture::m_ActiveTexture = 0;

	Texture::Texture(const char* location, bool generateMipMaps)
	{
		glGenTextures(1, &m_TextureID);
		m_ImageData = stbi_load(location, &m_ImageWidth, &m_ImageHeight, &m_ImageChannels, 0);
		if (m_ImageData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_ImageWidth, m_ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_ImageData);
			if (generateMipMaps) {
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
		else {
			std::cout << "[ERROR] Failed to load texture" << std::endl;
		}
		stbi_image_free(m_ImageData);
	}

	Texture::~Texture()
	{
		delete[] m_ImageData;
	}

	void Texture::Bind()
	{
		if (!m_Bound) {
			if (m_ActiveTexture < 16)
			{
				glActiveTexture(GL_TEXTURE0 + m_ActiveTexture);
				m_ActiveTexture++;
				glBindTexture(GL_TEXTURE_2D, m_TextureID);
				m_Bound = true;
			}
			else
			{
				std::cout << "[ERROR] Failed to bind texture. Too many texture's bound" << std::endl;
			}
		}
	}

	void Texture::UnBind()
	{
		if (m_Bound) {
			glBindTexture(GL_TEXTURE_2D, 0);
			m_ActiveTexture--;
		}
	}
}