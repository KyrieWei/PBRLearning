#pragma once
#include "../objects/FrameBuffer.h"
#include "../tools/Camera.h"
class DeferredShading
{
private:
	unsigned int screenQuadIndex;
	unsigned int deferredShaderIndex;
	
	FrameBuffer::ptr deferredFramebuffer;

public:
	typedef std::shared_ptr<DeferredShading> ptr;

	DeferredShading(int width, int height);
	~DeferredShading() = default;

	void bindDeferredFramebuffer();
	void renderDeferredShading(Camera::ptr camera);
};

