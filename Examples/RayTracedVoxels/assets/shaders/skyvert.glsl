#version 330 core
layout (location = 0) in vec3 a_Pos;

out vec3 v_TexCoord;

uniform mat4 u_TransformMatrix;

void main()
{
    gl_Position = u_TransformMatrix * vec4(a_Pos, 1.0);
    v_TexCoord = a_Pos;
} 