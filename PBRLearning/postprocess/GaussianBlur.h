#pragma once
#include "../textures/TextureMgr.h"
#include "../objects/FrameBuffer.h"

class GaussianBlur
{
private:
	unsigned int blurTimes;
	unsigned int readIndex;
	unsigned int writeIndex;
	unsigned int screenQuadIndex;
	unsigned int mergeShaderIndex;
	unsigned int gaussianShaderIndex;
	FrameBuffer::ptr framebuffer[2];

public:
	typedef std::shared_ptr<GaussianBlur> ptr;

	GaussianBlur(int width, int height);
	~GaussianBlur() = default;

	void bindGaussianFramebuffer();
	void renderGaussianBlurEffect();

};

