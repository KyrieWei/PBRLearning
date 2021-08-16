#version 330 core

out vec4 fragColor;

in vec2 TexCoord;

const float PI = 3.14159265359;

struct PointLight
{
	float radius;
	vec3 position;
	vec3 radiance;
};

uniform vec3 viewPos;

//lighting
#define MAX_LIGHT_NUM 128
uniform int pointLightNum;
uniform PointLight pointLight[MAX_LIGHT_NUM];
uniform float lightAttenuationCoeff;

//pbr material texture
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gOverall;

void main()
{
	//sample gbuffer
	vec3 FragPos = texture(gPosition, TexCoord).rgb;
	vec3 normal = texture(gNormal, TexCoord).rgb;
	vec3 albedo = texture(gAlbedo, TexCoord).rgb;
	vec3 overall = texture(gOverall, TexCoord).rgb;
	float roughness = overall.r;
	float metallic = overall.g;
	float depth = overall.b;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	
	//point light

	//ambient
	vec3 ambient = 0.15 * albedo;
	vec3 pointLightRadiance = vec3(0.0f);
	for(int i = 0; i < pointLightNum; i ++)
	{
		vec3 lightDir = normalize(pointLight[i].position - FragPos);
		vec3 halfwayDir = normalize(lightDir + viewDir);

		float distance = length(pointLight[i].position - FragPos);
		if(distance > pointLight[i].radius)
			continue;
		float attenuation = 1.0f / (lightAttenuationCoeff * distance * distance + 0.00001);
		vec3 radiance = pointLight[i].radiance * attenuation;

		//diffuse
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = diff * radiance;

		//specular
		float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
		vec3 specular = spec * radiance;

		pointLightRadiance += (diffuse + specular);
	}

	vec3 result = ambient + pointLightRadiance;

	//tone mapping
	vec3 col = vec3(1.0) - exp(- result * 1.0);
	col = pow(col, vec3(0.45));
	
	fragColor = vec4(col, 1.0);
}
