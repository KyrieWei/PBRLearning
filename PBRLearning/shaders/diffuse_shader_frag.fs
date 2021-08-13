#version 330 core

in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D albedoMap;

void main()
{
	//vec3 color = texture(albedoMap, TexCoord).rbg;
	vec3 color = vec3(0.3, 0.1, 0.7);
	fragColor = vec4(color, 1.0);
}