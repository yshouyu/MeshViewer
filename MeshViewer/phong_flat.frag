#version 440

uniform vec4 AmbientColor;
uniform float AmbientStrength;
uniform vec3 LightPos;
uniform vec4 DiffuseColor;
uniform vec4 SpecularColor;
uniform vec3 CameraPos;
uniform mat4x4 model;
uniform mat4x4 view;
uniform mat4x4 projection;
uniform mat4x4 glprojection;
uniform mat4x4 glmv;
flat in vec3 WorldPos;
flat in vec4 Color;
flat in vec3 WorldNormal;
out vec4 FragColor;

void main()  
{
	// ambient
    vec4 ambient = AmbientColor * AmbientStrength;

    // diffuse
    vec3 lightDir = normalize(LightPos - WorldPos);
    float diffuseFactor = max(dot(WorldNormal, lightDir), 0.0);
    vec4 diffuse = diffuseFactor * DiffuseColor;

    // specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(CameraPos - WorldPos);
	vec3 reflectDir = reflect(-lightDir, WorldNormal);
	float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec4 specular = specularStrength * specularFactor * SpecularColor;
	
    FragColor = Color * (ambient + diffuse + specular);
}