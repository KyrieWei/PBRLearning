#pragma once
#include <string>

#include "../tools/Camera.h"
#include "../objects/MeshMgr.h"
#include "../shaders/ShaderMgr.h"
#include "../textures/TextureMgr.h"

class PBRenderer
{
private:
	int width, height;

	Camera::ptr camera;

	MeshMgr::ptr meshMgr;
	ShaderMgr::ptr shaderMgr;
	TextureMgr::ptr textureMgr;

public:
	typedef std::shared_ptr<PBRenderer> ptr;
	

public:
	PBRenderer() = default;

	Camera::ptr getCamera(){ return camera; }
	MeshMgr::ptr getMeshMgr() { return meshMgr; }
	ShaderMgr::ptr getShaderMgr() { return shaderMgr; }
	TextureMgr::ptr getTextureMgr() { return textureMgr; }

	void initialzie(int width, int height);
	void render();

};

