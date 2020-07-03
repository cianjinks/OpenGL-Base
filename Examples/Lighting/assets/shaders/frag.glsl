#version 410 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_Pos;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;
uniform mat4 u_ModelMatrix;

void main()
{

	// Diffuse
	vec3 norm = mat3(transpose(inverse(u_ModelMatrix))) * v_Normal;
	vec3 lightDir = normalize(u_LightPos - v_Pos);

	float diff = max(dot(norm, lightDir), 0.0); //Max is used to ensure not negative when angle > 90 degrees 
	vec3 diffuse = diff * u_LightColor;

	// Ambient
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * u_LightColor;

	// Specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(u_CameraPos - v_Pos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * u_LightColor;  


	vec3 result = (ambient + diffuse + specular) * u_ObjectColor;

	color = vec4(result, 1.0);
}