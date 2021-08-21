#include "SSAOProcess.h"

#include "../shaders/ShaderMgr.h"
#include "../objects/MeshMgr.h"
#include "../textures/TextureMgr.h"

SSAOProcess::SSAOProcess(int width, int height, unsigned int screenQuadIndex) : screenQuadIndex(screenQuadIndex)
{
	ssaoFramebuffer = std::shared_ptr<FrameBuffer>(new FrameBuffer(width, height, "ssaoDepth", {}, true));
	ssaoShaderIndex = ShaderMgr::getSingleton()->loadShader("pbrSSAOProcess", "shaders/pbrSSAOProcess_vert.vs", "shaders/pbrSSAOProcess_frag.fs");
	ssaoBlurShaderIndex = ShaderMgr::getSingleton()->loadShader("pbrSSAOBlur", "shaders/pbrSSAOBlur_vert.vs", "shaders/pbrSSAOBlur_frag.fs");
	ssaoLinearizeShaderIndex = ShaderMgr::getSingleton()->loadShader("pbrSSAOLinearize", "shaders/pbrSSAOLinearize_vert.vs", "shaders/pbrSSAOLinearize_frag.fs");
	
}

void SSAOProcess::SSAOProcessDepth(Camera::ptr camera, unsigned int posTexIndex, unsigned int normalTexIndex, unsigned int depthTexIndex)
{

}