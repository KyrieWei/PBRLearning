#pragma once
#include <memory>
#include <string>

#include <glm/glm.hpp>

class Texture
{
protected:
	std::string name;
	unsigned int id;

public:
	typedef std::shared_ptr<Texture> ptr;

	Texture() = default;
	virtual ~Texture() = default;

	virtual void bind(unsigned int unit) = 0;
	virtual void unbind() = 0;

	unsigned int getTextureID() { return id; }

private:
	virtual void setupTexture(const std::string& path, const std::string& pFix) = 0;
	virtual void clearTexture() = 0;

};

class Texture2D : public Texture
{
private:
	int width,  height, channel;
	glm::vec4 borderColor;

public:
	typedef std::shared_ptr<Texture2D> ptr;

	Texture2D(const std::string& path, glm::vec4 bColor = glm::vec4(1.0f));
	Texture2D(unsigned char* images, int width, int height, int channel);
	~Texture2D();

	virtual void bind(unsigned int unit);
	virtual void unbind();

private:
	virtual void setupTexture(const std::string& path, const std::string& pFix);
	virtual void clearTexture();
};

class Texture2DHDR : public Texture
{
private:
	int width, height, channel;

public:
	typedef std::shared_ptr<Texture2DHDR> ptr;

	Texture2DHDR(const std::string& path);
	Texture2DHDR(const char* images, int width, int height);
	~Texture2DHDR();

	virtual void bind(unsigned int unit);
	virtual void unbind();

private:
	virtual void setupTexture(const std::string& path, const std::string& pFix);
	virtual void clearTexture();
};

class TextureCube : public Texture
{
public:
	TextureCube(const std::string& path, const std::string& posFix);
	~TextureCube();

	virtual void bind(unsigned int unit);
	virtual void unbind();

private:
	virtual void setupTexture(const std::string& path, const std::string& pFix);
	virtual void clearTexture();
};

class TextureCubeHDR : public Texture
{
public:
	TextureCubeHDR(const char* data, int width, int height, bool mipmap = false);
	~TextureCubeHDR();

	virtual void bind(unsigned int unit);
	virtual void unbind();

private:
	virtual void setupTexture(const std::string& path, const std::string& pFix);
	virtual void clearTexture();
};


class TextureColor : public Texture
{
private:
	bool hdr;
	int width, height;

public:
	TextureColor(int width, int height, bool hdr = false);
	~TextureColor();

	virtual void bind(unsigned int unit);
	virtual void unbind();

private:
	virtual void setupTexture(const std::string& path, const std::string& pFix);
	virtual void clearTexture();
};

class TextureDepth : public Texture
{
private:
	int width, height;

public:
	TextureDepth(int width, int height);
	~TextureDepth();

	virtual void bind(unsigned int unit);
	virtual void unbind();

private:
	virtual void setupTexture(const std::string& path, const std::string& pFix);
	virtual void clearTexture();
};