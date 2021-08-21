#pragma once
#include "../system/Light.h"
#include "../tools/Camera.h"
#include "../objects/FrameBuffer.h"

class SSAOProcess
{
private:
	std::vector<glm::vec3> samples;
	unsigned int noiseTexIndex;
	unsigned int screenQuadIndex;
	unsigned int ssaoShaderIndex;
	unsigned int ssaoBlurShaderIndex;
	unsigned int ssaoLinearizeShaderIndex;
	FrameBuffer::ptr ssaoFramebuffer;

public:
	typedef std::shared_ptr<SSAOProcess> ptr;

	SSAOProcess(int width, int height, unsigned int screenQuadIndex);
	~SSAOProcess() = default;

	void SSAOProcessDepth(Camera::ptr camera, unsigned int posTexIndex, unsigned int normalTexIndex, unsigned int depthTexIndex);

private:
	float randFloat();
	float lerp(float a, float b, float value);

	void generateSamples();
	void generateRandomNoiseTex();
};

