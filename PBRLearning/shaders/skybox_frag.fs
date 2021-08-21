#version 330 core

in vec3 localPos;

uniform samplerCube environmentMap;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

void main()
{
	vec3 envColor = textureLod(environmentMap, localPos, 1.0).rgb;

	float brightness = dot(envColor, vec3(0.2126, 0.7152, 0.0722));

	if(brightness > 0.4f)
		brightColor = vec4(envColor, 1.0f);

	//tone mapping
	envColor = vec3(1.0f) - exp(-envColor * 1.0);

	//gamma correction
	const float gamma = 2.2f;
	envColor = pow(envColor, vec3(1.0f / gamma));

	fragColor = vec4(envColor, 1.0f);
}