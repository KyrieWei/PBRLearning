#include "Light.h"

void DirectionalLight::setLightUniform(Shader::ptr shader, Camera::ptr camera)
{
	shader->setVec3("dirLight.direction", direction);
	shader->setVec3("dirLight.radiance", radiance);
}

PointLight::PointLight()
{
	attenuationCoeff = 0.5f;
	radiance = glm::vec3(1.0f);
	radius = sqrt(256.0f * 1.0f / (1.0f * attenuationCoeff));
}

void PointLight::setAttenuationCoeff(float coeff)
{
	attenuationCoeff = coeff;

	GLfloat lightMax = std::fmax(std::fmax(radiance.r, radiance.g), radiance.b);
	radius = sqrt(256.0f * lightMax / (1.0f * attenuationCoeff));
}

void PointLight::setLightUniform(Shader::ptr shader, Camera::ptr camera)
{
	shader->setVec3("pointLight[" + std::to_string(order) + "].position", position);
	shader->setVec3("pointLight[" + std::to_string(order) + "].radiance", radiance);
	shader->setFloat("pointLight[" + std::to_string(order) + "].radius", radius);
	shader->setFloat("lightAttenuationCoeff", attenuationCoeff);
}

