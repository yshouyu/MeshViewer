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
in vec4 Color;
in vec4 ambient;
in vec4 diffuse;
out vec4 FragColor;

void main()  
{
    FragColor = Color * (ambient + diffuse);
}