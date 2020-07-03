#version 410 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_Pos;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;
uniform vec3 u_CameraPos;
uniform mat4 u_ModelMatrix;

uniform float u_AmbientStrength;
uniform float u_SpecularStrength;
uniform int u_SpecularShininess;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material u_Material;

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light u_Light;

void main()
{
	// Ambient
	vec3 ambient = u_Light.ambient * u_Material.ambient;

	// Diffuse
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_Light.position - v_Pos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = u_Light.diffuse * (diff * u_Material.diffuse);

	// Specular
	vec3 viewDir = normalize(u_CameraPos - v_Pos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	vec3 specular = u_Light.specular * (spec * u_Material.specular); 

	vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
	color = vec4(result, 1.0f);
}