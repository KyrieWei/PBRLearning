#pragma once
#include <vector>
#include <memory>
#include "../tools/Shader.h"
#include "../tools/Camera.h"
#include "Transform3D.h"
#include "../system/Light.h"
#include "../shaders/ShaderMgr.h"

class PBRMaterial
{
public:
	unsigned int albedoTexIndex = -1;
	unsigned int normalTexIndex = -1;
	unsigned int roughTexIndex = -1;
	unsigned int metallicIndex = -1;
};

class Drawable
{
protected:
	unsigned int shaderIndex;
	std::vector<unsigned int> meshIndex;
	std::vector<PBRMaterial> texIndex;

	bool visible = true;
	Transform3D transformation;

public:
	typedef std::shared_ptr<Drawable> ptr;

	Drawable() = default;
	virtual ~Drawable() = default;

	virtual void render(Camera::ptr camera) = 0;
	virtual void renderDepth(Shader::ptr shader) = 0;

	void setVisible(bool target) { visible = target; }
	bool isVisible() const { return visible; }
	Transform3D* getTransformation() { return &transformation; }

	void addPbrTexture(PBRMaterial matIndex) { texIndex.push_back(matIndex); }
	void addMesh(unsigned int meshIndex) { this->meshIndex.push_back(meshIndex); }

protected:
	void renderImp();
};

class DrawableList : public Drawable
{
private:
	std::vector<Drawable::ptr> list;

public:
	typedef std::shared_ptr<DrawableList> ptr;

	DrawableList() = default;
	virtual ~DrawableList() = default;

	unsigned int addDrawable(Drawable* ptr)
	{
		Drawable::ptr drawable(ptr);
		list.push_back(drawable);
		return list.size() - 1;
	}

	virtual void render(Camera::ptr camera)
	{
		for (auto& it : list)
			it->render(camera);
	}

	virtual void renderDepth(Shader::ptr shader)
	{
		for (auto& it : list)
			it->renderDepth(shader);
	}
};

class SimpleDrawable : public Drawable
{
public:
	SimpleDrawable(unsigned int shaderIndex)
	{
		this->shaderIndex = shaderIndex;
	}

	~SimpleDrawable() = default;

	virtual void render(Camera::ptr camera);
	virtual void renderDepth(Shader::ptr shader);
};

class SkyDome : public Drawable
{
public:
	typedef std::shared_ptr<SkyDome> ptr;

	SkyDome(unsigned int shaderIndex)
	{
		this->shaderIndex = shaderIndex;
	}

	SkyDome() = default;

	virtual void render(Camera::ptr camera);
	virtual void renderDepth(Shader::ptr shader);
};

class PointLightDrawable : public Drawable
{
private:
	glm::vec3 lightColor;
	float particleRadius;
	unsigned int particleNum;
	unsigned int particleVAO;
	unsigned int particleVBO;
	ShaderMgr::ptr shaderMgr;

	struct LightSource
	{
		glm::vec3 pos;
		glm::vec3 color;
	};

public:
	typedef std::shared_ptr<PointLightDrawable> ptr;

	PointLightDrawable();
	~PointLightDrawable();

	void setPointLightRadius(float radius);
	void setPointLightPositions(const std::vector<PointLight::ptr>& lights);
	
	virtual void render(Camera::ptr camera);
	virtual void renderDepth(Shader::ptr shader);
};