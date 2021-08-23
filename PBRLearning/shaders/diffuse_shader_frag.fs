#version 330 core

in vec3 FragPos;
in vec2 TexCoord;
in mat3 TBNMatrix;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gOverall;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;

void main()
{
	gPosition = FragPos;
	
	vec3 normal = normalize(2.0f * texture(normalMap, TexCoord).rgb - vec3(1.0f));
	gNormal = TBNMatrix * normal;
    
	gAlbedo = texture(albedoMap, TexCoord).rgb;

	float roughness = texture(roughnessMap, TexCoord).r;
	float metallic = texture(metallicMap, TexCoord).r;
	float depth = gl_FragCoord.z;

	gOverall = vec3(roughness, metallic, depth);
}