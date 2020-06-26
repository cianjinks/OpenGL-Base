#include "Vertex.h"

namespace GLBase {

	int Vertex::PositionElementCount = 3;
	int Vertex::TextureElementCount = 2;
	int Vertex::TotalElementCount = 5;

	Vertex::Vertex(float* vertex)
		: m_X(vertex[0]), m_Y(vertex[1]), m_Z(vertex[2]), m_S(vertex[3]), m_T(vertex[4]) {}

	Vertex::Vertex(float x, float y, float z, float s, float t)
		: m_X(x), m_Y(y), m_Z(z), m_S(s), m_T(t) {}

	Vertex::~Vertex()
	{
	}

	float Vertex::GetX()
	{
		return m_X;
	}
	float Vertex::GetY()
	{
		return m_Y;
	}
	float Vertex::GetZ()
	{
		return m_Z;
	}
	float Vertex::GetS()
	{
		return m_S;
	}
	float Vertex::GetT()
	{
		return m_T;
	}

	std::vector<float> Vertex::GetVertex()
	{
		return { m_X, m_Y, m_Z, m_S, m_T};
	}
}