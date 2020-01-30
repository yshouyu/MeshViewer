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
in vec4 VertexPosition;  
in vec4 VertexColor;
in vec4 VertexNormal;
out vec4 Color;
out vec3 WorldNormal;
out vec3 WorldPos;
  
void main()  
{  
    Color = VertexColor;
    WorldNormal = normalize(mat3(transpose(inverse(model))) * vec3(VertexNormal));
    WorldPos = vec3(model * VertexPosition);
   	gl_Position = glprojection * view * model * VertexPosition;
} 