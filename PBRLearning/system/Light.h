#pragma once

#include <glm/glm.hpp>
#include "../tools/Camera.h"
#include "../tools/Shader.h"

class Light
{
protected:
	glm::vec3 radiance; 

public:
	typedef std::shared_ptr<Light> ptr;

	Light() = default;
	~Light() = default;

	glm::vec3 getRadiance() const { return radiance; }

	virtual void setLightColor(glm::vec3 radiance) { this->radiance = radiance; }
	virtual void setLightUniform(Shader::ptr shader, Camera::ptr camera) = 0;
};

class DirectionalLight : public Light
{
private:
	glm::vec3 direction;

public:
	typedef std::shared_ptr<DirectionalLight> ptr;

	DirectionalLight() = default;
	~DirectionalLight() = default;

	glm::vec3 getDirection() const { return direction; }

	void setDirection(glm::vec3 dir) { direction = glm::normalize(dir); }

	virtual void setLightUniform(Shader::ptr shader, Camera::ptr camera);
};

class PointLight : public Light
{
private:
	glm::vec3 position;
	float radius;
	unsigned int order;
	float attenuationCoeff;

public:
	typedef std::shared_ptr<PointLight> ptr;

	PointLight();
	~PointLight() = default;

	void setAttenuationCoeff(float coeff);
	virtual void setLightColor(glm::vec3 radiance) { this->radiance = radiance; }
	void setPosition(glm::vec3 pos, unsigned int order) { this->order = order; position = pos; }
	virtual void setLightUniform(Shader::ptr shader, Camera::ptr camera);

	glm::vec3 getPosition() const { return position; }
};

