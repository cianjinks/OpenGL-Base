#pragma once
#include <vector>
#include "Quad.h"

namespace GLBase
{
	class Cube
	{
	private:
		Quad* m_Quad1 = nullptr;
		Quad* m_Quad2 = nullptr;
		Quad* m_Quad3 = nullptr;
		Quad* m_Quad4 = nullptr;
		Quad* m_Quad5 = nullptr;
		Quad* m_Quad6 = nullptr;
		float m_X, m_Y, m_Z, m_Scale;

	public:

		Cube(float x, float y, float z, float scale);
		~Cube();

		void ClearQuads();
		std::vector<Quad*> GetQuads();

		float GetX();
		float GetY();
		float GetZ();
		float GetScale();

		static int VerticesPerCube;
	};
}