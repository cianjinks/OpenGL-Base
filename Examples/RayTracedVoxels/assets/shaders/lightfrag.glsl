#version 410 core
layout(location = 0) out vec4 color;

uniform vec3 u_LightColor;
void main()
{
	color = vec4(u_LightColor, 1.0);
}