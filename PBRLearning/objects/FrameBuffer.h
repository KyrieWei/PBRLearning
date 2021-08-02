#pragma once
#include <vector>
#include <string>
#include <memory>

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
private:
	void clearFramebuffer();
};

