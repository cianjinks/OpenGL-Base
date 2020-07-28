#version 410 core
layout(location = 0) in vec2 a_Pos;

out vec4 v_Pos;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(a_Pos, 0.0f, 1.0f);
	v_Pos = u_MVP * vec4(a_Pos, 0.0f, 1.0f);
}