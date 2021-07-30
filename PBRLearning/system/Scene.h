#pragma once
#include "PBRenderer.h"

class Scene
{
public:
	typedef std::shared_ptr<Scene> ptr;

	Scene() = default;

	virtual ~Scene() = default;

	virtual void initializeScene(PBRenderer::ptr pbrenderer) = 0;
};

class simpleScene : public Scene
{
public:
	typedef std::shared_ptr<simpleScene> ptr;

	simpleScene() = default;
	virtual ~simpleScene() = default;

	virtual void initializeScene(PBRenderer::ptr pbrenderer);

};