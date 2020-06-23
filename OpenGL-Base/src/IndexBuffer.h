#pragma once

namespace GLBase {
	class IndexBuffer
	{
	private:
		unsigned int* m_Indices;
		unsigned int m_NumIndices;
		unsigned int m_BufferID;
	public:
		IndexBuffer(unsigned int* indices, unsigned int numIndices);
		~IndexBuffer();

		void Bind();
		void UnBind();
	};
}