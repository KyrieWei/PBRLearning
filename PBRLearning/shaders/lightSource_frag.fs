#version 430 core

in vec3 Color;

out vec4 fragColor;

void main()
{
	vec3 normal;
	normal.xy = gl_PointCoord.xy * vec2(2.0, -2.0) + vec2(-1.0, 1.0);
	float mag = dot(normal.xy, normal.xy);
	if(mag > 1.0)
		discard;

	fragColor = vec4(Color, 1.0);
}