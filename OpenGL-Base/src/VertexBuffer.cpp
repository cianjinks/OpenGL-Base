#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "VertexBuffer.h"

namespace GLBase {

	VertexBuffer::VertexBuffer(float* vertices, unsigned int numVertices)
		: m_Vertices(vertices), m_NumVertices(numVertices)
	{
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ARRAY_BUFFER, m_NumVertices * sizeof(float), m_Vertices, GL_STATIC_DRAW);
	}

	// TODO: Avoid data copying if possible
	VertexBuffer::VertexBuffer(std::vector<float>& vertices)
		: m_Vertices(vertices.data()), m_NumVertices(vertices.size())
	{
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ARRAY_BUFFER, m_NumVertices * sizeof(float), m_Vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		delete[] m_Vertices;
	}

	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void VertexBuffer::UnBind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::AddVertexAttribute(int index, int elementCount, bool normalised, int strideCount, int elementIndex)
	{
		glVertexAttribPointer(index, elementCount, GL_FLOAT, normalised, strideCount * sizeof(float), (void*)(elementIndex * sizeof(float)));
	}

	void VertexBuffer::ActivateVertexAttribute(int index)
	{
		glEnableVertexAttribArray(index);
	}

	void VertexBuffer::DisableVertexAttribute(int index)
	{
		glDisableVertexAttribArray(index);
	}

	unsigned int VertexBuffer::GetNumVertices()
	{
		return m_NumVertices;
	}
}