#include "DeferredShading.h"
#include "../objects/MeshMgr.h"
#include "../shaders/ShaderMgr.h"
#include "../objects/Geometry.h"
#include "../textures/TextureMgr.h"

DeferredShading::DeferredShading(int width, int height)
{
	screenQuadIndex = MeshMgr::getSingleton()->loadMesh(new ScreenQuad());
	deferredShaderIndex = ShaderMgr::getSingleton()->loadShader("deferredShader", "", "");
	deferredFramebuffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(width, height, "deferredDepth", { "deferredPos", "deferredNormal", "deferredAlbedo", "deferredRoughness" }, true));
}

void DeferredShading::bindDeferredFramebuffer()
{
	deferredFramebuffer->bind();
}

void DeferredShading::renderDeferredShading(Camera::ptr camera)
{
	Shader::ptr deferredShader = ShaderMgr::getSingleton()->getShader(deferredShaderIndex);

	deferredShader->use();
	deferredShader->setInt("gPostion", 0);
	deferredShader->setInt("gNormal", 1);
	deferredShader->setInt("gAlbedo", 2);
	deferredShader->setInt("gRoughness", 3);
	
	TextureMgr::ptr texMgr = TextureMgr::getSingleton();
	texMgr->bindTexture("deferredPos", 0);
	texMgr->bindTexture("deferredNormal", 1);
	texMgr->bindTexture("deferredAlbedo", 2);
	texMgr->bindTexture("deferredRoughness", 3);

	MeshMgr::getSingleton()->drawMesh(screenQuadIndex);

	texMgr->unbindTexture("deferredPos");
	texMgr->unbindTexture("deferredNormal");
	texMgr->unbindTexture("deferredAlbedo");
	texMgr->unbindTexture("deferredRoughness");
}