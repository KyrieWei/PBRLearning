#version 330 core

out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D debug_texture;

void main()
{
	vec3 color = texture(debug_texture, TexCoord).rgb;
	fragColor = vec4(color, 1.0);
}