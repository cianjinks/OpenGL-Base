#version 410 core
layout(location = 0) in vec3 a_Pos;

uniform mat4 u_TransformMatrix;

void main()
{
	gl_Position = u_TransformMatrix * vec4(a_Pos, 1.0f);
}