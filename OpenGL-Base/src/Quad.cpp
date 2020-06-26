#include "Quad.h"

namespace GLBase {

	Quad::Quad(float x, float y, float z, float scale, QuadDirection facing)
		: m_X(x), m_Y(y), m_Z(z), m_Scale(scale), m_Direction(facing)
	{
		switch (m_Direction) {
			case QuadDirection::PositiveX :
				v1 = new Vertex(x, y - (scale / 2), z + (scale / 2), 0.0f, 0.0f);
				v2 = new Vertex(x, y - (scale / 2), z - (scale / 2), 1.0f, 0.0f);
				v3 = new Vertex(x, y + (scale / 2), z - (scale / 2), 1.0f, 1.0f);
				v4 = new Vertex(x, y + (scale / 2), z + (scale / 2), 0.0f, 1.0f);
				break;
			case QuadDirection::NegativeX :
				v1 = new Vertex(x, y - (scale / 2), z - (scale / 2), 0.0f, 0.0f);
				v2 = new Vertex(x, y - (scale / 2), z + (scale / 2), 1.0f, 0.0f);
				v3 = new Vertex(x, y + (scale / 2), z + (scale / 2), 1.0f, 1.0f);
				v4 = new Vertex(x, y + (scale / 2), z - (scale / 2), 0.0f, 1.0f);
				break;
			case QuadDirection::PositiveY :
				v1 = new Vertex(x - (scale / 2), y, z + (scale / 2), 0.0f, 0.0f);
				v2 = new Vertex(x + (scale / 2), y, z + (scale / 2), 1.0f, 0.0f);
				v3 = new Vertex(x + (scale / 2), y, z - (scale / 2), 1.0f, 1.0f);
				v4 = new Vertex(x - (scale / 2), y, z - (scale / 2), 0.0f, 1.0f);
				break;
			case QuadDirection::NegativeY :
				v1 = new Vertex(x - (scale / 2), y, z - (scale / 2), 0.0f, 0.0f);
				v2 = new Vertex(x + (scale / 2), y, z - (scale / 2), 1.0f, 0.0f);
				v3 = new Vertex(x + (scale / 2), y, z + (scale / 2), 1.0f, 1.0f);
				v4 = new Vertex(x - (scale / 2), y, z + (scale / 2), 0.0f, 1.0f);
				break;
			case QuadDirection::PositiveZ :
				v1 = new Vertex(x - (scale / 2), y - (scale / 2), z, 0.0f, 0.0f);
				v2 = new Vertex(x + (scale / 2), y - (scale / 2), z, 1.0f, 0.0f);
				v3 = new Vertex(x + (scale / 2), y + (scale / 2), z, 1.0f, 1.0f);
				v4 = new Vertex(x - (scale / 2), y + (scale / 2), z, 0.0f, 1.0f);
				break;
			case QuadDirection::NegativeZ :
				v1 = new Vertex(x - (scale / 2), y - (scale / 2), z, 0.0f, 0.0f);
				v2 = new Vertex(x + (scale / 2), y - (scale / 2), z, 1.0f, 0.0f);
				v3 = new Vertex(x + (scale / 2), y + (scale / 2), z, 1.0f, 1.0f);
				v4 = new Vertex(x - (scale / 2), y + (scale / 2), z, 0.0f, 1.0f);
				break;
		}
	}

	Quad::~Quad()
	{
		// TODO: Might not work
		delete v1, v2, v3, v4;
	}

	void Quad::ClearVertices()
	{
		delete v1, v2, v3, v4;
	}

	std::vector<Vertex*> Quad::GetVertices()
	{
		return { v1, v2, v3, v4 };
	}

	float Quad::GetX()
	{
		return m_X;
	}

	float Quad::GetY()
	{
		return m_Y;
	}

	float Quad::GetZ()
	{
		return m_Z;
	}

	float Quad::GetScale()
	{
		return m_Scale;
	}
}