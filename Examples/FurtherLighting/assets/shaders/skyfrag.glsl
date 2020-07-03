#version 330 core
layout(location = 0) out vec4 color;

in vec3 v_TexCoord;

uniform samplerCube skybox;

void main()
{    
    //color = texture(skybox, v_TexCoord);
    color = vec4(vec3(0.0f), 1.0f);
}