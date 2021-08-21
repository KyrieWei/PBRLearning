#version 330 core

in vec2 TexCoord;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

uniform sampler2D Color;
uniform sampler2D Bright;

void main()
{
	vec3 result = texture(Bright, TexCoord).rgb;
	brightColor = vec4(result, 1.0);
	fragColor = texture(Color, TexCoord);
}