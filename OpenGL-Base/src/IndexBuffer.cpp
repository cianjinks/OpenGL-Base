#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "IndexBuffer.h"

namespace GLBase {

	IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int numIndices)
		: m_Indices(indices), m_NumIndices(numIndices) 
	{
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), m_Indices, GL_STATIC_DRAW);
	}
	
	IndexBuffer::~IndexBuffer()
	{
		
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

	void IndexBuffer::UnBind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}