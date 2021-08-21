#include "DeferredShading.h"
#include "../objects/MeshMgr.h"
#include "../shaders/ShaderMgr.h"
#include "../objects/Geometry.h"
#include "../textures/TextureMgr.h"

DeferredShading::DeferredShading(int width, int height)
{
	screenQuadIndex = MeshMgr::getSingleton()->loadMesh(new ScreenQuad());
	deferredShaderIndex = ShaderMgr::getSingleton()->loadShader("deferredShader", "shaders/pbrDeferredShading_vert.vs", "shaders/pbrDeferredShading_frag.fs");
	deferredFramebuffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(width, height, "deferredDepth", { "deferredPos", "deferredNormal", "deferredAlbedo", "deferredRoughness" }, true));
}

void DeferredShading::bindDeferredFramebuffer()
{
	deferredFramebuffer->bind();
}

unsigned int DeferredShading::getFrameBufferId()
{
	return deferredFramebuffer->getID();
}

void DeferredShading::ssaoFactorGeneration(Camera::ptr camera)
{
	unsigned int posTexIndex = TextureMgr::getSingleton()->getTextureIndex("deferredPos");
	unsigned int normalTexIndex = TextureMgr::getSingleton()->getTextureIndex("deferredNormal");
	unsigned int depthTexIndex = TextureMgr::getSingleton()->getTextureIndex("deferredDepth");
	ssaoProcess->SSAOProcessDepth(camera, posTexIndex, normalTexIndex, depthTexIndex);
}

void DeferredShading::renderDeferredShading(Camera::ptr camera, Light::ptr sunLight, const std::vector<PointLight::ptr>& pointLights)
{
	Shader::ptr deferredShader = ShaderMgr::getSingleton()->getShader(deferredShaderIndex);

	deferredShader->use();
	//gbuffer
	deferredShader->setInt("gPosition", 0);
	deferredShader->setInt("gNormal", 1);
	deferredShader->setInt("gAlbedo", 2);
	deferredShader->setInt("gOverall", 3);
	//IBL precompute
	deferredShader->setInt("irradianceMap", 4);
	deferredShader->setInt("prefilteredMap", 5);
	deferredShader->setInt("brdfLutMap", 6);

	deferredShader->setVec3("viewPos", camera->Position);

	//set light information
	sunLight->setLightUniform(deferredShader, camera);
	deferredShader->setInt("pointLightNum", pointLights.size());
	for (unsigned int i = 0; i < pointLights.size(); i++)
		pointLights[i]->setLightUniform(deferredShader, camera);
	
	TextureMgr::ptr texMgr = TextureMgr::getSingleton();
	//gbuffer
	texMgr->bindTexture("deferredPos", 0);
	texMgr->bindTexture("deferredNormal", 1);
	texMgr->bindTexture("deferredAlbedo", 2);
	texMgr->bindTexture("deferredRoughness", 3);
	//IBL precompute
	texMgr->bindTexture("irradianceMap", 4);
	texMgr->bindTexture("prefilteredMap", 5);
	texMgr->bindTexture("brdfLutMap", 6);

	MeshMgr::getSingleton()->drawMesh(screenQuadIndex);

	texMgr->unbindTexture("deferredPos");
	texMgr->unbindTexture("deferredNormal");
	texMgr->unbindTexture("deferredAlbedo");
	texMgr->unbindTexture("deferredRoughness");

	texMgr->unbindTexture("irradianceMap");
	texMgr->unbindTexture("prefilteredMap");
	texMgr->unbindTexture("brdfLutMap");

}