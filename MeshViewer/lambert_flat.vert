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
flat out vec4 Color;
flat out vec4 ambient;
flat out vec4 diffuse;
  
void main()  
{  
	vec3 worldPos = vec3(model * VertexPosition);
	vec3 worldNormal = normalize(mat3(transpose(inverse(model))) * vec3(VertexNormal));

	// ambient
    ambient = AmbientColor * AmbientStrength;

    // diffuse
    vec3 lightDir = normalize(LightPos - worldPos);
    float diffuseFactor = max(dot(worldNormal, lightDir), 0.0);
    diffuse = diffuseFactor * DiffuseColor;

    Color = VertexColor;
   	gl_Position = glprojection * view * model * VertexPosition;
} 