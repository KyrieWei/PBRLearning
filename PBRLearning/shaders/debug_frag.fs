#version 330 core

out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gOverall;

void main()
{
	vec3 color = texture(gNormal, TexCoord).rgb;
	fragColor = vec4(color, 1.0);
}