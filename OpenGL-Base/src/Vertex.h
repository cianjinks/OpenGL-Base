#pragma once
#include <vector>

namespace GLBase {

	class Vertex
	{
	private:
		float m_X = 0, m_Y = 0, m_Z = 0, m_S = 0, m_T = 0;
	public:
		static int PositionElementCount;
		static int TextureElementCount;
		static int TotalElementCount;
	public:
		Vertex(float* vertex);
		Vertex(float x, float y, float z, float s, float t);
		~Vertex();

		float GetX();
		float GetY();
		float GetZ();
		float GetS();
		float GetT();

		std::vector<float> GetVertex();
	};
}