#include "Vertex.h"

namespace GLBase {

	int Vertex::PositionElementCount = 3;
	int Vertex::TextureElementCount = 2;
	int Vertex::NormalElementCount = 3;
	int Vertex::TotalElementCount = 8;

	Vertex::Vertex(float* vertex)
		: m_X(vertex[0]), m_Y(vertex[1]), m_Z(vertex[2]), m_S(vertex[3]), m_T(vertex[4]), m_NX(vertex[5]), m_NY(vertex[6]), m_NZ(vertex[7]) {}

	Vertex::Vertex(float x, float y, float z, float s, float t, float nx, float ny, float nz)
		: m_X(x), m_Y(y), m_Z(z), m_S(s), m_T(t), m_NX(nx), m_NY(ny), m_NZ(nz) {}

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
	float Vertex::GetNX()
	{
		return m_NX;
	}
	float Vertex::GetNY()
	{
		return m_NY;
	}
	float Vertex::GetNZ()
	{
		return m_NZ;
	}

	std::vector<float> Vertex::GetVertex()
	{
		return { m_X, m_Y, m_Z, m_S, m_T, m_NX, m_NY, m_NZ };
	}
}