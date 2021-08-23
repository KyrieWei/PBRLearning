#version 330 core

in vec2 TexCoord;

const float PI = 3.14159265359;

struct DirLight
{
	vec3 direction;
	vec3 radiance;
};

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
uniform DirLight dirLight;
uniform PointLight pointLight[MAX_LIGHT_NUM];
uniform float lightAttenuationCoeff;

//pbr material texture
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gOverall;

uniform samplerCube irradianceMap;
uniform samplerCube prefilteredMap;
uniform sampler2D brdfLutMap;

float NormalDistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float costheta, vec3 F0);
vec3 fresnelSchlickRoughness(float costheta, vec3 F0, float roughness);

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

void main()
{
	//sample gbuffer
	vec3 FragPos = texture(gPosition, TexCoord).rgb;
	vec3 normal = texture(gNormal, TexCoord).rgb;
	vec3 albedo = pow(texture(gAlbedo, TexCoord).rgb, vec3(2.2));
	vec3 overall = texture(gOverall, TexCoord).rgb;
	float roughness = overall.r;
	float metallic = overall.g;
	float depth = overall.b;

	if(normal.x == 0.0f && normal.y == 0.0f && normal.z == 0.0f)
	{
		fragColor.rgb = albedo;

		float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
		brightColor = vec4(fragColor.rgb, 1.0f);

		gl_FragDepth = depth;
		return;
	}

	
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 F0 = vec3(0.04);				//电解质表面的平均菲涅尔反射率
	F0 = mix(F0, albedo, metallic);		//metallic 决定表面的金属度，影响菲涅尔反射率

	//----------------------------directional light-----------------------------------
	vec3 lightDir = dirLight.direction;
	vec3 halfwayDir = normalize(viewDir + lightDir);
	//fresnel factor
	vec3 fresnel = fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0f), F0);
	//normal distribution factor
	float distribution = NormalDistributionGGX(normal, halfwayDir, roughness);
	//geometry factor
	float geometryFactor = GeometrySmith(normal, viewDir, lightDir, roughness);
	//brdf function
	vec3 brdf = distribution * fresnel * geometryFactor / (4.0f * max(dot(viewDir, normal), 0.0f) * max(dot(lightDir, normal), 0.0f) + 0.0001f);
	vec3 kSpecular = fresnel;
	vec3 kDiffuse = vec3(1.0f) - kSpecular;
	kDiffuse *= (1.0f - metallic);
	//rendering equation
	vec3 directionalLightCol = (kDiffuse * albedo / PI + brdf) * dirLight.radiance * max(dot(normal, lightDir), 0.0f);

	//-----------------------------point light---------------------------------------
	vec3 pointLightRadiance = vec3(0.0f);
	for(int i = 0; i < pointLightNum; i ++)
	{
		vec3 lightDir = normalize(pointLight[i].position - FragPos);
		halfwayDir = normalize(viewDir + lightDir);
		float distance = length(pointLight[i].position - FragPos);
		if(distance > pointLight[i].radius)
			continue;
		float attenuation = 1.0f / (lightAttenuationCoeff * distance * distance + 0.00001f);
		vec3 radiance = pointLight[i].radiance * attenuation;

		vec3 fresnel = fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0f), F0);
		float distribution = NormalDistributionGGX(normal, halfwayDir, roughness);
		float geometryFactor = GeometrySmith(normal, viewDir, lightDir, roughness);
		//brdf function
		vec3 brdf = distribution * fresnel * geometryFactor / (4.0f * max(dot(viewDir, normal), 0.0f) * max(dot(lightDir, normal), 0.0f) + 0.0001f);
		vec3 kSpecular = fresnel;
		vec3 kDiffuse = vec3(1.0f) - kSpecular;
		kDiffuse *= (1.0f - metallic);
		//rendering equation
		pointLightRadiance += (kDiffuse * albedo / PI + brdf) * radiance * max(dot(normal, lightDir), 0.0f);
	}

	//--------------------------environment light-----------------------------------
	vec3 F = fresnelSchlickRoughness(max(dot(normal, viewDir), 0.0f), F0, roughness);
	
	vec3 KS = F;
	vec3 KD = vec3(1.0f) - KS;
	KD *= (1.0f - metallic);
	vec3 irradiance = texture(irradianceMap, normal).rgb;
	vec3 diffuse = irradiance * albedo;

	const float MAX_REFLECTION_LOD = 4.0;
	vec3 R = normalize(reflect(-viewDir, normal));
	vec3 prefilteredColor = textureLod(prefilteredMap, R, roughness * MAX_REFLECTION_LOD).rgb;
	vec2 envBrdf = texture(brdfLutMap, vec2(max(dot(normal, viewDir), 0.0f), roughness)).rg;
	vec3 envSpecular = prefilteredColor * (KS * envBrdf.x + envBrdf.y);
	vec3 ambient = KD * diffuse + envSpecular;

	vec3 col = ambient + pointLightRadiance + directionalLightCol;
	
	//blur map
	float brightness = dot(col / (col + vec3(1.0f)), vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 0.55f)
		brightColor = vec4(col / (col + vec3(1.0f)), 1.0f);

	//tone mapping
	col = vec3(1.0f) - exp(-col * 1.0);

	//gamma correction
	const float gamma = 2.2f;
	col = pow(col, vec3(1.0f / gamma));

	fragColor = vec4(col, 1.0);
	gl_FragDepth = depth;
}

vec3 fresnelSchlick(float costheta, vec3 F0)
{
	return F0 + (1.0f - F0) * pow(1.0 - costheta, 5.0f);
}

vec3 fresnelSchlickRoughness(float costheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(1.0 - costheta, 5.0f);
}

float NormalDistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2; 
}