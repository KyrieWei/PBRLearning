#include "FrameBuffer.h"

#include <iostream>

#include "../textures/TextureMgr.h"

FrameBuffer::FrameBuffer(int width_, int height_, const std::string& depthName, const std::vector<std::string>& colorName, bool hdr_) : width(width_), height(height_), hdr(hdr_)
{
	glGenFramebuffers(1, &id);

	//color attachment
	colorTexIndex.resize(colorName.size());
	for (int i = 0; i < colorName.size(); i++)
		setupColorFramebuffer(colorName[i], i);

	//depth attachment
	if (!depthName.empty())
		setupDepthFramebuffer(depthName);

	glBindFramebuffer(GL_FRAMEBUFFER, id);

	if (colorName.size() > 0)
		glDrawBuffers(colorName.size(), ColorAttachments);

	//unsigned int captureRBO;
	//glGenRenderbuffers(1, &captureRBO);

	//glBindFramebuffer(GL_FRAMEBUFFER, id);
	//glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

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

void FrameBuffer::setupColorFramebuffer(const std::string& name, unsigned int attachIndex)
{
	TextureMgr::ptr textureMgr = TextureMgr::getSingleton();
	colorTexIndex[attachIndex] = textureMgr->loadTextureColor(name, width, height, hdr);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachIndex, GL_TEXTURE_2D, textureMgr->getTexture(colorTexIndex[attachIndex])->getTextureID(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::setupDepthFramebuffer(const std::string& name)
{
	TextureMgr::ptr textureMgr = TextureMgr::getSingleton();
	depthTexIndex = textureMgr->loadTextureDepth(name, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureMgr->getTexture(depthTexIndex)->getTextureID(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::clearFramebuffer()
{
	glDeleteFramebuffers(1, &id);
}