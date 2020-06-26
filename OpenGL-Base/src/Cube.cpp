#include "Cube.h"

namespace GLBase
{
	Cube::Cube(float x, float y, float z, float scale)
		: m_X(x), m_Y(y), m_Z(z), m_Scale(scale)
	{
		m_Quad1 = new Quad(x, y, z + (scale / 2), scale, QuadDirection::PositiveZ); // Front face
		m_Quad2 = new Quad(x + (scale / 2), y, z, scale, QuadDirection::PositiveX); // Right side face
		m_Quad3 = new Quad(x, y, z - (scale / 2), scale, QuadDirection::NegativeZ); // Back side face
		m_Quad4 = new Quad(x - (scale / 2), y, z, scale, QuadDirection::NegativeX); // Left side face
		m_Quad5 = new Quad(x, y - (scale / 2), z, scale, QuadDirection::NegativeY); // Bottom side face
		m_Quad6 = new Quad(x, y + (scale / 2), z, scale, QuadDirection::PositiveY); // Top side face
	}

	Cube::~Cube()
	{
		delete m_Quad1, m_Quad2, m_Quad3, m_Quad4, m_Quad5, m_Quad6;
	}

	void Cube::ClearQuads()
	{
		delete m_Quad1, m_Quad2, m_Quad3, m_Quad4, m_Quad5, m_Quad6;
	}

	std::vector<Quad*> Cube::GetQuads()
	{
		return {m_Quad1, m_Quad2, m_Quad3, m_Quad4, m_Quad5, m_Quad6};
	}

	float Cube::GetX()
	{
		return m_X;
	}

	float Cube::GetY()
	{
		return m_Y;
	}

	float Cube::GetZ()
	{
		return m_Z;
	}

	float Cube::GetScale()
	{
		return m_Scale;
	}
}