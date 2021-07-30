#pragma once
#include <string>

#include "../tools/Camera.h"
#include "../objects/MeshMgr.h"

class PBRenderer
{
private:
	int width, height;

	Camera::ptr camera;

	MeshMgr::ptr meshMgr;

public:
	typedef std::shared_ptr<PBRenderer> ptr;
	

public:
	PBRenderer() = default;

	Camera::ptr getCamera(){ return camera; }
	MeshMgr::ptr getMeshMgr() { return meshMgr; }

	void initialzie(int width, int height);
	void render();

};

