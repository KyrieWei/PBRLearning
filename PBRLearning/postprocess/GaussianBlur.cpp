#include "GaussianBlur.h"
#include "../objects/Geometry.h"
#include "../objects/MeshMgr.h"
#include "../shaders/ShaderMgr.h"

GaussianBlur::GaussianBlur(int width, int height) : readIndex(0), writeIndex(1), blurTimes(10)
{
	framebuffer[0] = std::shared_ptr<FrameBuffer>(new FrameBuffer(width, height, "GaussianDepth0", { "Color0", "BrightColor0" }, true));
	framebuffer[1] = std::shared_ptr<FrameBuffer>(new FrameBuffer(width, height, "GaussianDepth1", { "Color1", "BrightColor1" }, true));

	screenQuadIndex = MeshMgr::getSingleton()->loadMesh(new ScreenQuad());
	mergeShaderIndex = ShaderMgr::getSingleton()->loadShader("MergeBlur", "shaders/mergeBlur_vert.vs", "shaders/mergeBlur_frag.fs");
	gaussianShaderIndex = ShaderMgr::getSingleton()->loadShader("GaussianBlur", "shaders/gaussianBlur_vert.vs", "shaders/gaussianBlur_frag.fs");
}

void GaussianBlur::bindGaussianFramebuffer()
{
	framebuffer[readIndex]->bind();
}

void GaussianBlur::renderGaussianBlurEffect()
{
	Shader::ptr blurShader = ShaderMgr::getSingleton()->getShader(gaussianShaderIndex);
	blurShader->use();
	
	framebuffer[writeIndex]->bind();
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	blurShader->setInt("Color", 0);
	blurShader->setInt("Bright", 1);
	TextureMgr::getSingleton()->bindTexture("Color" + std::to_string(readIndex), 0);
	TextureMgr::getSingleton()->bindTexture("BrightColor" + std::to_string(readIndex), 1);
	MeshMgr::getSingleton()->drawMesh(screenQuadIndex);

	framebuffer[writeIndex]->unBind();
	framebuffer[readIndex]->unBind();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	Shader::ptr mergeShader = ShaderMgr::getSingleton()->getShader(mergeShaderIndex);
	//TextureMgr::ptr depthMap = TextureMgr::getSingleton()->getTexture("shadowDepth");
	mergeShader->use();
	mergeShader->setInt("Color", 0);
	mergeShader->setInt("BrightColor", 1);
	TextureMgr::getSingleton()->bindTexture("Color0", 0);
	TextureMgr::getSingleton()->bindTexture("BrightColor0", 1);
	MeshMgr::getSingleton()->drawMesh(screenQuadIndex);
	TextureMgr::getSingleton()->unbindTexture("Color0");
	TextureMgr::getSingleton()->unbindTexture("BrightColor0");

}