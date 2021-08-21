#pragma once
#include "../objects/FrameBuffer.h"
#include "../tools/Camera.h"
#include "../system/Light.h"
#include "SSAOProcess.h"
class DeferredShading
{
private:
	unsigned int screenQuadIndex;
	unsigned int deferredShaderIndex;
	
	FrameBuffer::ptr deferredFramebuffer;
	SSAOProcess::ptr ssaoProcess;
public:
	typedef std::shared_ptr<DeferredShading> ptr;

	DeferredShading(int width, int height);
	~DeferredShading() = default;

	void bindDeferredFramebuffer();
	unsigned int getFrameBufferId();
	void ssaoFactorGeneration(Camera::ptr camera);
	void renderDeferredShading(Camera::ptr camera, Light::ptr sunLight, const std::vector<PointLight::ptr>& pointLights);

};

