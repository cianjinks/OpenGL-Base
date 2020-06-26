#version 410 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D containerTexture;

void main()
{
	color = texture(containerTexture, v_TexCoord);
}