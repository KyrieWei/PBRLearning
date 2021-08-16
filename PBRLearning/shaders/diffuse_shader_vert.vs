#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;
layout (location = 4) in vec3 tangent;
layout (location = 5) in vec3 bitangent;


out vec2 TexCoord;
out vec3 FragPos;
out mat3 TBNMatrix;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
	vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));

	TBNMatrix = mat3(T, B, N);

	FragPos = (model * vec4(aPos, 1.0)).rgb;
	TexCoord = aTexCoord;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}