#include "FrameBuffer.h"
#include <glad/glad.h>
#include <iostream>

FrameBuffer::FrameBuffer(int width_, int height_, const std::string& depthName, const std::vector<std::string>& colorName, bool hdr_) : width(width_), height(height_), hdr(hdr_)
{
	glGenFramebuffers(1, &id);

	unsigned int captureRBO;
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind()
{
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::unBind(int width_, int height_)
{
	if (width_ == -1) width_ = this->width;
	if (height_ == -1) height_ = this->height;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width_, height_);
}

void FrameBuffer::clearFramebuffer()
{
	glDeleteFramebuffers(1, &id);
}