#pragma once

namespace GLBase {
	class Texture
	{
	private:
		unsigned int m_TextureID;
		unsigned char* m_ImageData;
		bool m_Bound = false;

		int m_ImageWidth;
		int m_ImageHeight;
		int m_ImageChannels;

		static int m_ActiveTexture;
	public:
		Texture(const char* location, bool generateMipMaps);
		~Texture();

		void Bind();
		void UnBind();
	};
}