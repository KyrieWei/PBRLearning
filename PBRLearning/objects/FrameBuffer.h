#pragma once
#include <vector>
#include <string>
#include <memory>
#include <glad/glad.h>

const GLenum ColorAttachments[] =
{
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7
};

class FrameBuffer
{
private:
	bool hdr;
	unsigned int id;
	int width, height;
	unsigned int depthTexIndex;
	std::vector<unsigned int> colorTexIndex;

public:
	typedef std::shared_ptr<FrameBuffer> ptr;

	FrameBuffer(int width, int height, const std::string& depthName, const std::vector<std::string>& colorName, bool hdr = false);
	virtual ~FrameBuffer() { clearFramebuffer(); }

	void bind();
	void unBind(int width = -1, int height = -1);

	unsigned int getID() const { return id; }

private:
	void clearFramebuffer();
	void setupColorFramebuffer(const std::string& name, unsigned int attachIndex);
	void setupDepthFramebuffer(const std::string& name);
};

