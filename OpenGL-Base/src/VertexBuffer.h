#pragma once
#include <vector>
namespace GLBase {
	class VertexBuffer
	{
	private:
		float* m_Vertices;
		unsigned int m_NumVertices;
		unsigned int m_BufferID;

	public:
		VertexBuffer(float* vertices, unsigned int numVertices);
		VertexBuffer(std::vector<float>& vertices);
		~VertexBuffer();

		void Bind();
		void UnBind();
		
		void AddVertexAttribute(int id, int elementCount, bool normalised, int strideCount, int elementIndex);
		void ActivateVertexAttribute(int id);
		void DisableVertexAttribute(int id);

		unsigned int GetBufferID();
		unsigned int GetNumVertices();
	};
}