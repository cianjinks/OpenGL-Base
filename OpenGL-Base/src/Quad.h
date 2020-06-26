#pragma once
#include "Vertex.h"
#include <vector>

namespace GLBase {

	enum class QuadDirection {
		PositiveX = 0,
		NegativeX,
		PositiveY,
		NegativeY,
		PositiveZ,
		NegativeZ
	};

	class Quad
	{
	private:
		GLBase::Vertex* v1 = nullptr;
		GLBase::Vertex* v2 = nullptr;
		GLBase::Vertex* v3 = nullptr;
		GLBase::Vertex* v4 = nullptr;
		float m_X, m_Y, m_Z, m_Scale;
		QuadDirection m_Direction;

	public:
		Quad(float x, float y, float z, float scale, QuadDirection facing);
		~Quad();

		void ClearVertices();
		std::vector<Vertex*> GetVertices();

		float GetX();
		float GetY();
		float GetZ();
		float GetScale();
	};
}