#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "IndexBuffer.h"

namespace GLBase {

	IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int numIndices)
		: m_Indices(indices), m_NumIndices(numIndices)
	{
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumIndices * sizeof(unsigned int), m_Indices, GL_STATIC_DRAW);
	}

	IndexBuffer::IndexBuffer(std::vector<unsigned int>& indices)
		: m_Indices(indices.data()), m_NumIndices(indices.size())
	{
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumIndices * sizeof(unsigned int), m_Indices, GL_STATIC_DRAW);
	}
	
	IndexBuffer::~IndexBuffer()
	{
		delete[] m_Indices;
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

	void IndexBuffer::UnBind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	unsigned int IndexBuffer::GetNumIndices()
	{
		return m_NumIndices;
	}
}