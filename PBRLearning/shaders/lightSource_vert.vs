#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float pointScale;
uniform float pointSize;

out vec3 Color;

void main()
{
	Color = aColor;
	vec3 eyeSpacePos = (view * model * vec4(aPos, 1.0)).xyz;
	gl_PointSize = max(-pointScale * pointSize / eyeSpacePos.z, 0.0);
	gl_Position = projection * vec4(eyeSpacePos, 1.0);
}