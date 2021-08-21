#pragma once
#include <string>

#include "../tools/Camera.h"
#include "../objects/MeshMgr.h"
#include "../shaders/ShaderMgr.h"
#include "../textures/TextureMgr.h"
#include "../drawable/Drawable.h"
#include "../postprocess/DeferredShading.h"
#include "../postprocess/GaussianBlur.h"
#include "Light.h"

class PBRenderer
{
private:
	int width, height;

	//skydome
	SkyDome::ptr skyDome;
	DirectionalLight::ptr sunLight;
	std::vector<PointLight::ptr> pointLights;

	Camera::ptr camera;

	//bloom effect
	GaussianBlur::ptr gaussianBlur;
	//deferred shading
	DeferredShading::ptr deferredShading;
	//light
	PointLightDrawable::ptr pointLightDrawable;

	//manager
	MeshMgr::ptr meshMgr;
	ShaderMgr::ptr shaderMgr;
	TextureMgr::ptr textureMgr;
	DrawableList::ptr drawableList;


public:
	typedef std::shared_ptr<PBRenderer> ptr;
	

public:
	PBRenderer() = default;

	Camera::ptr getCamera(){ return camera; }
	MeshMgr::ptr getMeshMgr() { return meshMgr; }
	ShaderMgr::ptr getShaderMgr() { return shaderMgr; }
	TextureMgr::ptr getTextureMgr() { return textureMgr; }

	void initialzie(int width, int height);
	void setSkyDomeHDR(const std::string& path);
	void addPointLight(glm::vec3 pos, glm::vec3 radiance);
	void addPointLightDrawble();
	void addDrawable(Drawable* target) { drawableList->addDrawable(target); }
	void setSunLight(glm::vec3 dir, glm::vec3 radiance);

	void render_simplescene();
	void render();
};

