#version 410 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

out vec3 v_Normal;
out vec3 v_Pos;

uniform mat4 u_TransformMatrix;
uniform mat4 u_ModelMatrix;

void main()
{
	gl_Position = u_TransformMatrix * vec4(a_Pos, 1.0f);
	v_Normal = mat3(transpose(inverse(u_ModelMatrix))) * a_Normal;
	v_Pos = vec3(u_ModelMatrix * vec4(a_Pos, 1.0f));
}