#include "PBRenderer.h"

void PBRenderer::initialzie(int _width, int _height)
{
	width = _width;
	height = _height;

	camera = std::make_shared<Camera>(Camera(glm::vec3(0.0f, 0.0f, 3.0f)));

	meshMgr = MeshMgr::getSingleton();

}

void PBRenderer::render()
{
	meshMgr->drawMesh(0);
}

